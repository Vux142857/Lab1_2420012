#!/bin/bash

ANS_FILE="$HOME/.calc_ans"
HIST_FILE="$HOME/.calc_history"

if [ ! -f "$ANS_FILE" ]; then
    echo "0" > "$ANS_FILE"
fi

ANS=$(cat "$ANS_FILE")

save_history() {
    local entry="$1"
    if [ -f "$HIST_FILE" ]; then
        (echo "$entry"; cat "$HIST_FILE") | head -n 5 > "$HIST_FILE.tmp" && mv "$HIST_FILE.tmp" "$HIST_FILE"
    else
        echo "$entry" > "$HIST_FILE"
    fi
}

show_history() {
    if [ -f "$HIST_FILE" ]; then
        cat "$HIST_FILE"
    else
        echo "No history available"
    fi
}

calculate() {
    local input="$1"
    local num1 oper num2

    read num1 oper num2 <<< "$input"

    if ! [[ "$num1" =~ ^[0-9]*\.?[0-9]+$ || "$num1" == "ANS" ]] || \
       ! [[ "$num2" =~ ^[0-9]*\.?[0-9]+$ ]] || \
       ! [[ "$oper" =~ ^[\+\-x/%]$ ]]; then
        echo "SYNTAX ERROR"
        return 1
    fi

    if [ "$num1" == "ANS" ]; then
        num1="$ANS"
    fi

    if [ "$oper" == "/" ] && [ "$num2" == "0" ]; then
        echo "MATH ERROR"
        return 1
    fi

    case "$oper" in
        "+") res=$(echo "scale=2; $num1 + $num2" | bc);;
        "-") res=$(echo "scale=2; $num1 - $num2" | bc);;
        "x") res=$(echo "scale=2; $num1 * $num2" | bc);;
        "/") res=$(echo "scale=2; $num1 / $num2" | bc);;
        "%") res=$(echo "scale=0; $num1 % $num2" | bc);;
    esac

    if [[ "$res" =~ \.00$ ]]; then
        res=${res%.00}
    fi

    echo "$res"
    echo "$res" > "$ANS_FILE"
    save_history "$num1 $oper $num2 = $res"
    return 0
}

while true; do
    echo -n ">> "
    read input

    if [ "$input" == "EXIT" ]; then
        exit 0
    elif [ "$input" == "HIST" ]; then
        show_history
        read -s -n 1
        clear
    else
        calculate "$input"
        if [ $? -eq 0 ]; then
            read -s -n 1
            
            clear
        fi
    fi
done