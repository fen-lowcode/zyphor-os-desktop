# shellcheck shell=bash
# Shared helpers for zyphor-smart-downloads.

ZSD_CONF=/etc/zyphor-smart-downloads/zyphor-smart-downloads.conf

# to_seconds 30s|5m|1h|45  -> prints seconds, returns 1 on bad input
to_seconds() {
    local v=${1// /} n u
    [[ $v =~ ^([0-9]+)([smh]?)$ ]] || return 1
    n=${BASH_REMATCH[1]}; u=${BASH_REMATCH[2]}
    case $u in
        m) echo $((n * 60)) ;;
        h) echo $((n * 3600)) ;;
        *) echo "$n" ;;
    esac
}

load_config() {
    ENABLED=yes
    SCAN_INTERVAL=1m
    MIN_FILE_AGE=10s
    STABILITY_WAIT=2s
    DATE_SOURCE=birth
    DATE_FORMAT='%Y%m%d-%H%M%S'
    INCLUDE_USERS=""
    EXCLUDE_USERS=""
    UNKNOWN_FILES=leave
    DAMAGED_ACTION=quarantine
    # shellcheck disable=SC1090
    [ -r "$ZSD_CONF" ] && . "$ZSD_CONF"
    return 0
}
