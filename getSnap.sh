SNAP_HOST_IP=172.20.10.7
OUTFILE1=$1
OUTFILE2=$2
curl "http://${SNAP_HOST_IP}:8001/snapshot?username=admin&password=admin&strm=1&q=50" --output ${OUTFILE1}
sleep 0.5
curl "http://${SNAP_HOST_IP}:8001/snapshot?username=admin&password=admin&strm=1&q=50" --output ${OUTFILE2}

