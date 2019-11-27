# Intruduction

This is the Java SDK for OffLineSignLib.

## Installation

1. Before you use the java SDK, please make sure you have installed the library to server. Please refer [here](../README.md "Installation") to get the installation steps.
2. Install java sdk to local

``` shell
./gradlew clean build uploadArchives
```

1. Add the java sdk dependency to you code.

- For gradle

``` groovy
compile 'com.bigbang:bigbang-core:1.0.0'
```

- For maven

``` xml
<dependency>
    <groupId>com.bigbang</groupId>
    <artifactId>bigbang-core</artifactId>
    <version>1.0.0</version>
</dependency>
```

## Usage

Create BigBangCore object to get the operations.

``` java
BigBangCore bigbang = BigBangCore.builder().build();
bigbang.makeKeyPair();
```

The default rpc is lisentening at http://127.0.0.1:9902, if the rpc of bigbang wallet core is not listentening at this url. You should set it to BigBangCore.

``` java
BigBangCore bigbang = BigBangCore.builder().rpcUrl("http://127.0.0.1:19902").build();
bigbang.makeKeyPair();
```
