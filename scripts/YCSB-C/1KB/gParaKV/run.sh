#!/bin/bash

ulimit -n 999999

/home/ubuntu/My/ParaKV/cmake-build-debug/bin/ycsbc -filename /media/test -db leveldb -configpath 0 -P /home/ubuntu/My/GPURocksDB-V3/workloads/100/workloadb.spec &> ./b.txt

/home/ubuntu/My/ParaKV/cmake-build-debug/bin/ycsbc -filename /media/test -db leveldb -configpath 0 -P /home/ubuntu/My/GPURocksDB-V3/workloads/100/workloadd.spec &> ./d.txt

/home/ubuntu/My/ParaKV/cmake-build-debug/bin/ycsbc -filename /media/test -db leveldb -configpath 0 -P /home/ubuntu/My/GPURocksDB-V3/workloads/100/workloadf.spec &> ./f.txt

#/home/ubuntu/My/ParaKV/cmake-build-debug/bin/ycsbc -filename /media/test -db leveldb -configpath 0 -P /home/ubuntu/My/GPURocksDB-V3/workloads/100/workloada.spec &> ./a.txt

/home/ubuntu/My/ParaKV/cmake-build-debug/bin/ycsbc -filename /media/test -db leveldb -configpath 0 -P /home/ubuntu/My/GPURocksDB-V3/workloads/100/workloadc.spec &> ./c.txt

/home/ubuntu/My/ParaKV/cmake-build-debug/bin/ycsbc -filename /media/test -db leveldb -configpath 0 -P /home/ubuntu/My/GPURocksDB-V3/workloads/100/workloade.spec &> ./e.txt

