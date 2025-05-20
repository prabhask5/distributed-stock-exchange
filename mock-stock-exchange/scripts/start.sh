#!/bin/bash

source ../../env.sh

trap cleanup 1 2 3 6

cleanup()
{
	echo "Caught Signal ... cleaning up..."

	$EXCHANGE_BASEDIR/scripts/stop.sh

	echo "Done."

	exit 1;
}

sleep 1
$DSE_HOME/scripts/dataservice.sh start data_service_a.ini

sleep 1
$DSE_HOME/scripts/dataservice.sh start data_service_b.ini

sleep 1
$DSE_HOME/scripts/matchingengine.sh start matching_engine_a.ini

sleep 1
$DSE_HOME/scripts/matchingengine.sh start matching_engine_b.ini

sleep 1
$DSE_HOME/scripts/matchingengine.sh start matching_engine_c.ini

sleep 1
$DSE_HOME/scripts/fixgateway.sh start fix_gwy_a.ini

sleep 1
$DSE_HOME/scripts/fixgateway.sh start fix_gwy_b.ini

sleep 1
$DSE_HOME/scripts/fixgateway.sh start fix_gwy_c.ini

while true; do 
	$DSE_HOME/scripts/dataservice.sh check data_service_a.ini
	$DSE_HOME/scripts/dataservice.sh check data_service_b.ini
	$DSE_HOME/scripts/matchingengine.sh check matching_engine_a.ini
	$DSE_HOME/scripts/matchingengine.sh check matching_engine_b.ini
	$DSE_HOME/scripts/matchingengine.sh check matching_engine_c.ini
	$DSE_HOME/scripts/fixgateway.sh check fix_gwy_a.ini
	$DSE_HOME/scripts/fixgateway.sh check fix_gwy_b.ini
	$DSE_HOME/scripts/fixgateway.sh check fix_gwy_c.ini

	sleep 2;
done