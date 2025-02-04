#! /bin/bash

if [  -n "${ZSH_VERSION:-}" ]; then 
	DIR="$(readlink -f -- "${(%):-%x}")"
	DIRNAME="$(dirname $DIR)"
	GAP_SDK_HOME=$(dirname $DIRNAME)
	export GAP_SDK_HOME
	#echo $(dirname "$(readlink -f ${(%):-%N})")
else
	export GAP_SDK_HOME="$(dirname $(dirname "$(readlink -f "${BASH_SOURCE[0]}")"))"
fi

export TARGET_CHIP_FAMILY="GAP8"
export TARGET_CHIP="GAP8_V2"
export TARGET_NAME="gap_rev1"
export BOARD_NAME=gapoc_b
export PULP_CURRENT_CONFIG=$BOARD_NAME@config_file=config/$BOARD_NAME.json
export GVSOC_CONFIG=$BOARD_NAME
export OPENOCD_CHIP_TARGET=target/gap8revb.tcl
export OPENOCD_CABLE=interface/ftdi/olimex-arm-usb-ocd.cfg

source $GAP_SDK_HOME/configs/common.sh
