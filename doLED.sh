SNAP_HOST_IP=172.20.10.7
ONOFF="0"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=0&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=1&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=2&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=3&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=4&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=5&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=6&on=${ONOFF}" #solid blue
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=7&on=${ONOFF}"
curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=8&on=${ONOFF}"

THRESHOLD=$1

if [ "${THRESHOLD}" -gt "10.000" ] ; then 
	curl "http://${SNAP_HOST_IP}:8001/ledctrl?status=6&on=1" #solid blue
fi