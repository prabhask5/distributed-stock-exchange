#!/bin/bash

. $DSE_HOME/env.sh

exec &>$DSE_HOME/logs/matchingengine.script.log.txt
set -x

SCRIPT=$0
PROGRAM_NAME=MatchingEngine
CONFIG_FILE_NAME=$2

LOG_FILE_NAME="$DSE_HOME/logs/$PROGRAM_NAME.$CONFIG_FILE_NAME.console.log"
PROCESS="$PROGRAM_NAME -c ${DSE_HOME}${EXCHANGE_BASEDIR}/config/$CONFIG_FILE_NAME"

USER_ID_NUM=$(id -u)

case $1 in
    start)
        echo "Starting : $PROCESS"
        $0 check $CONFIG_FILE_NAME
        RETURN_CODE=$?

        # This block only executes if the process has not already started.
        if [ "$RETURN_CODE" == "1" ]
		then
			$DSE_HOME/bin/$PROCESS >> $LOG_FILE_NAME 2>&1 & # Starts the process in the background.
			sleep 1
		fi

		$0 check $CONFIG_FILE_NAME
        RETURN_CODE=$?

        # The following logic checks if the program has successfully started.
        if [ "$RETURN_CODE" == "0" ]
        then
            echo "$PROGRAM_NAME successfully started."
        else
            echo "There was an error starting $PROGRAM_NAME, try again."
        fi
    ;;
    stop)
        echo "Stopping : $PROCESS"
        pkill -SIGTERM -f "$PROCESS"
        sleep 1
        $0 check $CONFIG_FILE_NAME
        RETURN_CODE=$?

        if [ "$RETURN_CODE" == "0" ]
		then
			cnt=0
			while [ true ]
			do
                PID=$(pgrep -U $USER_ID_NUM -f  "$PROCESS")
                if [ "$PID" == "" ]
                then
                    break
                fi
                sleep 1
                if [ $cnt -gt 10 ]
                then
                    break
                fi
                cnt=$(($cnt+1))
			done

			pkill -KILL -U $USER_ID_NUM -f "$PROCESS"
		fi
    ;;
    check)
        echo "Checking : $PROCESS"
        PID=$(pgrep -f "$PROCESS")
        if [ ! -z "$PID" ]
        then
            echo "$PROGRAM_NAME [$CONFIG_FILE_NAME] is running - $PID."
            exit 0
        else
            echo "$PROGRAM_NAME [$CONFIG_FILE_NAME] is not running."
            exit 1
        fi
    ;;
    *)
        echo "Unknown command."
        exit -1;
    ;;
esac

echo "$(date '+%Y%m%d.%H%M%S') $PROGRAM_NAME run done: pid,$PID."