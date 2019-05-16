#!/bin/bash
# Prints the system's public ip address.

# Arguments to use if able to load IP over DNS using dig:
DNS_ARGS='@resolver1.opendns.com -4 ANY myip.opendns.com +short'
# Arguments to use if requesting IP over curl:
CURL_ARGS='-s -4 icanhazip.com'

if [ `command -v dig` ]; then
    ADDR=`dig $DNS_ARGS`
fi

if [ -z "$ADDR" ]; then
    ADDR=`curl $CURL_ARGS`
fi

echo $ADDR
