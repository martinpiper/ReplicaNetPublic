## Docker RNLobby services


### For postgres

docker pull postgres
docker run --name some-postgres -e POSTGRES_PASSWORD=mysecretpassword -d -p 5432:5432 postgres
docker inspect -f "{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}" some-postgres
psql -U postgres
create database rnlobby;

Or restart with: docker start some-postgres


### For RNLobby services (from source)

docker -D compose up --force-recreate --build

docker run -it rnlobbydockerservices-app /bin/bash
docker run -it rnlobbydockerservices-app /bin/sh

docker run -it rnlobbydockerservices-app RNSQL

docker run -d rnlobbydockerservices-app RLSrvMonitor /d -file /root/RLSrvMonitor.html
docker run -d -p 4000:4000/udp -p 4001:4001/udp rnlobbydockerservices-app RLSrvBackend /d -dstart -addr 0.0.0.0:4001
docker run -d -p 4004:4004/udp rnlobbydockerservices-app RLSrvNATDetect /d -addr 0.0.0.0:4004 -addr2 0.0.0.0:4005 -addr 0.0.0.0:4006 -addr2 0.0.0.0:4007
docker run -d -p 4002:4002/udp rnlobbydockerservices-app RLSrvNAT /d -dstart
docker run -d -p 4008:4008/udp rnlobbydockerservices-app RLSrvAdvertise /d -dstart
docker run -d -p 4009:4009/udp rnlobbydockerservices-app RLSrvUser /d -dstart
docker run -d -p 4010:4010/udp rnlobbydockerservices-app RLSrvStat /d -dstart
docker run -d -p 4003:4003/udp rnlobbydockerservices-app RLSrvCode /d -dstart -file /root/cdkeys.txt
docker run -d -p 2000:2000 rnlobbydockerservices-app RLSrvUtility /d -dstart
docker run -d rnlobbydockerservices-app RLSrvWatchdog /d -dstart
>> Not used in Linux: docker run -d rnlobbydockerservices-app RLSrvSecurity /d -securityRoute 88.208.234.1 -dstart


	http://localhost:2000/getInformation
	http://localhost:2000/AccountObfuscatePassword?name=foo&password=123


### For RNLobby services (From docker hub)

docker pull martinpiper/rnlobbydockerservices-app:0.0.1

docker run -it martinpiper/rnlobbydockerservices-app:0.0.1 RNSQL

>> Not used in Linux: docker run -d martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvMonitor /d -file /root/RLSrvMonitor.html
docker run -d -p 4000:4000/udp -p 4001:4001/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvBackend /d -dstart -addr 0.0.0.0:4001
docker run -d -p 4004:4004/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvNATDetect /d -addr 0.0.0.0:4004 -addr2 0.0.0.0:4005 -addr 0.0.0.0:4006 -addr2 0.0.0.0:4007
docker run -d -p 4002:4002/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvNAT /d -dstart
docker run -d -p 4008:4008/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvAdvertise /d -dstart
docker run -d -p 4009:4009/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvUser /d -dstart
docker run -d -p 4010:4010/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvStat /d -dstart
docker run -d -p 4003:4003/udp martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvCode /d -dstart -file /root/cdkeys.txt
docker run -d -p 2000:2000 martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvUtility /d -dstart
>> Not used in Linux: docker run -d martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvWatchdog /d -dstart
>> Not used in Linux: docker run -d martinpiper/rnlobbydockerservices-app:0.0.1 RLSrvSecurity /d -securityRoute 88.208.234.1 -dstart



## For Kubernetes services

cd ReplicaNetWork\RNLobby\Kubernetes
kubectl apply -f .

## For Kubernetes load balancer without service manifests

kubectl expose deployment rnlobby-rlsrvutility-deployment --type=LoadBalancer --name=rnlobby-rlsrvutility
kubectl expose deployment rnlobby-rlsrvbackend-deployment --type=LoadBalancer --name=rnlobby-rlsrvbackend
kubectl expose deployment rnlobby-rlsrvnatdetect-deployment --type=LoadBalancer --name=rnlobby-rlsrvnatdetect
kubectl expose deployment rnlobby-rlsrvnat-deployment --type=LoadBalancer --name=rnlobby-rlsrvnat
kubectl expose deployment rnlobby-rlsrvadvertise-deployment --type=LoadBalancer --name=rnlobby-rlsrvadvertise
kubectl expose deployment rnlobby-rlsrvuser-deployment --type=LoadBalancer --name=rnlobby-rlsrvuser
kubectl expose deployment rnlobby-rlsrvstat-deployment --type=LoadBalancer --name=rnlobby-rlsrvstat
kubectl expose deployment rnlobby-rlsrvcode-deployment --type=LoadBalancer --name=rnlobby-rlsrvcode
