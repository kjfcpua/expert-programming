# docker

## 基本命令

### docker

#### pull
```shell
# Alpine Linux is a Linux distribution built around musl libc and BusyBox. 
docker pull alpine:latest
```

#### image
```shell
# list
docker image list
# rm all image
docker image list | awk 'FNR>1{print $3}' | xargs -I{} docker image rm {}
# rm all <none> image
docker image list | grep "<none>" | awk 'FNR>1{print $3}' | xargs -I{} docker image rm {}
# save to disk
docker save -o ${path} ${image_id}
# load from disk
docker load -i ${path}
```

#### containter
```shell
# list all containter
docker ps -a
# rm containter by name
docker rm ${name}
# rm containter by id
docker rm ${container_id}
# stop all containter
docker ps -a | awk 'FNR>1{print $1}' | xargs -I{} docker stop {}
# rm all containter
docker ps -a | awk 'FNR>1{print $1}' | xargs -I{} docker rm {}
```

#### run
```shell
docker run --name "centos" -it centos /bin/bash
docker run --name "ubuntu" -v ~/share/:/mnt/share -it ubuntu /bin/bash
docker run --name "nginx" -d -p 80:80 nginx
docker run --name "redis" -d -p 6379:6379 redis
docker run --name "mysqld" -d -p 3306:3306 -e MYSQL_ROOT_PASSWORD=123456 mysql
```

#### start

```shell
docker start ${CONTAINER_ID}
```

#### attach

```shell
docker attach --sig-proxy=false ${CONTAINER_ID}
```

#### tag
```shell
docker tag nginx 127.0.0.1/test/nginx
```

#### commit
```shell
docker commit ${CONTAINER_ID} ${REPO_TAG}
```

#### push

### compose
#### run
```shell
docker-compose up
```

#### login
```shell
docker login 127.0.0.1
```

#### push
```shell
docker push 127.0.0.1/test/nginx
```

#### pull
```shell
docker pull 127.0.0.1/test/nginx
```

#### build
```shell
git clone git@github.com:docker/distribution.git
docker build -t="baidubce/registry" .
```

### machine

#### delete
```shell
docker-machine rm default
```

#### create
```shell
docker-machine create --driver virtualbox default
docker-machine create --driver vmwarefusion default
```

#### start
```shell
docker-machine start default
```

#### set env
```shell
eval $(docker-machine env)
docker info # check it env
```

### ref
#### curl
```shell
curl -XGET ${host}/v2/_catalog # get all images
curl -XGET ${host}/v2/${image}/tags/list # get image's tags
curl -XGET ${host}/v2/${image}/manifests/${tag} # get tag's manifests
```

## FAQ

### RequestTimeTooSkewed

#### Error Message
```
Code: RequestTimeTooSkewed; Message: The difference between the request time and the server's time is too large.;
```

#### Solution - set docker env
```shell
docker run -it centos /bin/bash
date '+%s'
```
如果容器中的date与宿主的date不相同，则必然会导致客户端时间与服务器时间不相同。
最常见的原因是docker默认machine driver HyperKit可能会出现容器时间异常，且无法通过外部参数设置容器的时间。

```shell
# add virtualbox machine
docker-machine create --driver virtualbox default
# init shell env
eval $(docker-machine env)
```

### HTTP 403

#### Error Message
```
error parsing HTTP 403 response body: unexpected end of JSON input: ""
```

#### Solution - disable redirect
在S3的协议及兼容协议中，由于在Authorization中包含了Method的信息，GET和HEAD不是幂等的，可能会导致Redirect请求出现`403 Forbidden`的错误。
```
environment:
  - REGISTRY_STORAGE_REDIRECT_DISABLE=true
```
