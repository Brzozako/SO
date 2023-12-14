#!/bin/bash

echo "Podaj (-l/-b)"
read opcja

case "$opcja" in
    "-l")
        for module in /sys/module/*/refcnt; do
            if [ -f "$module" ]; then
                module_name=${module%/*}
                module_name=${module_name##*/}
                module_users=$(<"$module")
                using_modules=$(find "/sys/module/$module_name/holders" -type l -printf "%f," 2>/dev/null | sed "s/,$//")
                echo "$module_name $module_users $using_modules"
            fi
        done | sort
        ;;

    "-b")
        for module in /sys/module/*; do
            if [ ! -d "$module/refcnt" ]; then
                module_name=${module##*/}
                module_parameters=$(find "/sys/module/$module_name/parameters" -printf "%f," 2>/dev/null | sed "s/,$//")
                echo "$module_name $module_parameters"
            fi
        done | sort
        ;;

    *)
        echo "Wybrano złą opcję"
        exit 1
        ;;
esac
