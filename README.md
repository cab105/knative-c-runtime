# Knative C Runtime

Built on the Kaniko/Knative approach of building a runtime environment for injecting a C/C++ based serverless function so that it could be loaded into a Knative enabled Kubernetes Environment.

# Building the service

Take a look at the [example-dump](example-dump/) directory for a sample "hello world" style application  that can provide some http connection details. The only file that would need to be modified is `app.c`.

The example directory is setup to use docker to perform the build, however the header files in [runtime/httpstatus.h](runtime/httpstatus.h/) and [runtime/knativeruntime.h](runtime/knativeruntime.h) can be used.  To build using make on the local system, the `knativeruntime.a` will need to be built and copied into the service directory.

# Running the service

The example service can be deployed in a Kubernetes cluster running [Knative](https://knative.dev) and [Tekton pipeline](https://github.com/tektoncd/pipeline).

The sample `serverless.yaml` file references a [Kubernetes base docker registry](https://github.com/triggermesh/knative-local-registry) by default, but this can be changed to reflect the user's preferred registry.

The service can be deployed using the [Triggermesh CLI](https://github.com/triggermesh/tm) as:

```shell
$ cd knative-c-runtime/example-dump && tm deploy
Uploading "." to dump-service-dump-function-mxx7j-pod-e74534
Waiting for taskrun "dump-service-dump-function-mxx7j" ready state
Deployment started. Run "tm -n default describe service dump-service-dump-function" to see details
```

The initial build process will take several minutes to complete, and will help to
monitor the progress of the service using `tm get service`.

# Building the runtime

The build can be done using docker, or it can be built using make.

The Knative C Runtime is dependent upon [libhttp](https://github.com/lammertb/libhttp). Clone the repo, and use the master branch to build the libhttp.a.  Note that this may require applying a patch to build successfully. From the root of the libhttp source directory, copy the `lib/libhttp.a` file and `include/libhttp.h` file, and place it in the runtime directory.  Then build the runtime library using `make`.

# TODO

* Add a malloc/free library to track per-request allocations to prevent leaks in
  a thread-safe manner