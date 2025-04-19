#!/bin/bash

sleep 1
$DSE_HOME/data-service/scripts/dataservice.sh stop data_service_a.ini

sleep 1
$DSE_HOME/data-service/scripts/dataservice.sh stop data_service_b.ini

sleep 1
$DSE_HOME/matching-engine/scripts/matchingengine.sh stop matching_engine_a.ini

sleep 1
$DSE_HOME/matching-engine/scripts/matchingengine.sh stop matching_engine_b.ini

sleep 1
$DSE_HOME/matching-engine/scripts/matchingengine.sh stop matching_engine_c.ini

sleep 1
$DSE_HOME/fix-gateway/scripts/fixgateway.sh stop fix_gwy_a.ini

sleep 1
$DSE_HOME/fix-gateway/scripts/fixgateway.sh stop fix_gwy_b.ini

sleep 1
$DSE_HOME/fix-gateway/scripts/fixgateway.sh stop fix_gwy_c.ini