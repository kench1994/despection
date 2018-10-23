#! /bin/bash
Phone='192.168.100.100'
Target='syberos'
sb2 -t $Target-target-armv7tnhl rpmbuild --build-in-place -bb ../deviceInspection.spec
#sb2 -t $Target-target-armv7tnhl qmake && sb2 -t $Target-target-armv7tnhl make

scp /home/hekun/rpmbuild/RPMS/armv7tnhl/deviceInspection-0.8.0.w302-1.armv7tnhl.rpm developer@$Phone:/data/developer/
#scp ./deviceInspection developer@$Phone:/data/developer
