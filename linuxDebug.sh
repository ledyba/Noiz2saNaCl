CURDIR=$(cd $(dirname $0);pwd)
gnome-terminal -- python2 -m SimpleHTTPServer
cat > /tmp/script.gdb <<EOF
nacl-manifest ${CURDIR}/noiz2sa.nmf
nacl-file ${CURDIR}/noiz2sa_64.nexe
EOF
${NACL_ROOT}/chrome-linux/chrome "--nacl-gdb=gnome-terminal -- ${NACL_ROOT}/nacl-gdb/bin/gdb" "--nacl-gdb-script=/tmp/script.gdb" http://localhost:8000/chrome_extension/

rm /tmp/script.gdb
