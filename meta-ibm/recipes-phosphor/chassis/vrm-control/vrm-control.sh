#!/bin/bash
# #########################################################
# Script to run on witherspoon BMC to read/set vrm voltages


# #########################################################
function d2v() {
    # usage:   d2v <decimal volts>

    echo "$1" | awk '{print $1 * 256 + .5}' | cut -d '.' -f 1
}


# #########################################################
function v2d() {
    # usage:   v2d <hex val>

    printf "	%0.3fV" "$(echo "$1" | awk '{print $1 / 256}')"
}


# #########################################################
function i2d() {
    # usage:  i2d <hex val> <current divisor>

    # parse current mantisa and exponent
    # format: SEEE ESMM MMMM MMMM
    e=$(( $1/0x800 ))
    esign=$(( e/0x10 ))
    m=$(( $1 & 0x07FF ))
    msign=$(( m/0x0400 ))

    if [ $msign -eq 1 ]
    then
        # calc ones compliment
        m=$(( (m^0x07FF)+1 ))
        m=$(( -m ))
    fi
    if [ $esign -eq 1 ]
    then
        # calc ones compliment
        e=$(( (e^0x1F)+1 ))
        e=$(( -e ))
    fi
    printf "	%0.3fA\n" "$(echo $m $e "$2" | awk '{print ($1 * 2^$2)}')"
}


# #########################################################
function rw_vc() {
    # usage:   rw_vc <bus> <addr> <current divisor> <channel> <value>

    # select channel
    if [ "$4" != "x" ]
    then
        i2cset -y "$1" "$2" 0 "$4" b
    fi

    # write new voltage set point
    if [ ! -e "$5" ]
    then
        i2cset -y "$1" "$2" 0x21 "$(d2v "$5")" w
    fi

    # print voltage set point
    v2d "$(i2cget -y "$1" "$2" 0x21 w)"

    # print voltage
    v2d "$(i2cget -y "$1" "$2" 0x8B w)"

    # print current
    i2d "$(i2cget -y "$1" "$2" 0x8C w)" "$3"

    # default back to channel 0
    if [ "$4" != "x" ]
    then
        i2cset -y "$1" "$2" 0 0 b
    fi
}


# #########################################################
# Main

if [ -e "$1" ]
then
    $0 vdda vcsa vdna vioa vddra vppa vddb vcsb vdnb viob vddrb vppb
    exit
fi

if [ "$1" == "-h" ]
then
    echo "  Usage: vrm [<rail>=[value] [<rail>=[value]] ...]"
    echo "    rail: vdda vcsa vdna vioa vddra vppa vddb vcsb vdnb viob vddrb vppb vdnd viod"
    echo "    value: volts"
    echo
    echo "    e.g., vrm vioa=1.0 viob=1.0"
    echo
    exit
fi

echo "rail	set	read	current"
echo "-------	------- ------- -------"
for param in "${@:1}"
do
    rail=$(echo "$param" | cut -d'=' -f 1)
    val=$(echo "${param}=" | cut -d'=' -f 2)
    echo -n "$rail"
    case "$rail" in
        vdda)
            rw_vc 4 0x70 2 0 "$val"
            ;;
        vddb)
            rw_vc 5 0x70 2 0 "$val"
            ;;
        vcsa)
            rw_vc 4 0x70 4 1 "$val"
            ;;
        vcsb)
            rw_vc 5 0x70 4 1 "$val"
            ;;
        vdna)
            rw_vc 4 0x71 2 0 "$val"
            ;;
        vdnb)
            rw_vc 5 0x71 2 0 "$val"
            ;;
        vioa)
            rw_vc 4 0x40 2 x "$val"
            ;;
        viob)
            rw_vc 5 0x40 2 x "$val"
            ;;
        vddra)
            rw_vc 4 0x71 2 1 "$val"
            ;;
        vddrb)
            rw_vc 5 0x71 2 1 "$val"
            ;;
        vppa)
            rw_vc 12 0x41 2 x "$val"
            ;;
        vppb)
            rw_vc 13 0x41 2 x "$val"
            ;;
        vdnd)
            rw_vc 2 0x70 2 0 "$val"
            ;;
        viod)
            rw_vc 2 0x70 2 1 "$val"
            ;;
        *)
            echo "	non-existant"
    esac
done
