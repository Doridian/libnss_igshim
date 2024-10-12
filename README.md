# libnss_igshim

## Purpose of this library

This library provides a minimal shim from the `*grent` functions of a given NSS module to `initgroups_dyn`

## Reason for this library

Some libnss modules only provide a subset of functionality. 

Specifically when it comes to groups, sometimes only `setgrent`, `endgrent` and `getgrent` functionality to iterate all groups.

However, for auxiliary user groups to work, `initgroups` must also be defined, which gives groups for a specified user.

On glibc, `libnss_compat` takes care of providing one from the other.

However, alpine, using musl-nscd, of course does not have `libnss_compat`

## Installation (Alpine)

Put the following in `/etc/apk/keys/libnss_igshim_ci.key.pub`:
```
-----BEGIN PUBLIC KEY-----
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAuAC4tUAAAragbNqe9fxA
TcggPQa122bP0Aj/xt7qFZVSgQ45z1qpzvgC/zGsKDmF3XuwBRUiFigs/Ru9u8eH
WOSyf7I2bS8csCWU7G2x7PT4wqB0i5Qgyy0hDA1SyCyf4ACFRC1GxLWC1TIiDTqb
CsGL1EOQdtrEZpwJdsxCqV73/kWejKfSxRADMM/wGOWmQMo9oc0k4FPuHfg4Nmnh
5JYzjkrjLtUUbB6+eZSydd3f3v7HxocE6iOubWwgvu5DWmrytapNO7gNxG0bDZqa
2f7Ih+YVs0j8zWgLyXHfAf6Ey852yVnx//fkVKuxe7MXFxFGwCZK5RHp3iUfRzwC
VjNUN4mAtzNpSAMeT00fxgyWntyfKeqaOIrbCam5RZ+4KmBuJfmGVua7YxEUL49A
1fdFOKdDCCyHXDFbvoytBG6J19NTzEO3j4mjILGIsEva4XcWqZbusGVcZPdYO3Lo
AW4rhmFqPCfYWCesFXbYms21Sawx+9BmC1ViQi+jlBdElA82yteQAPK83rWABGl2
pdUUHlg573E2W37cilga83PdOVm5dRRsjw/boBpaZtezS+SaGl9RkZKMKY+VB0sv
bdXd5GWNIfj/irE7guGz9NXByBX5DftuEvJDx9tPXCRqYslObpiP/GynYDi84KYJ
gxKDJ9EPV895CTIABEfpKvsCAwEAAQ==
-----END PUBLIC KEY-----
```

Add the following line to `/etc/apk/repositories`:
```
https://github.com/Doridian/libnss_igshim/releases/download
```

Run the following shell command as root (or via `sudo` / `doas`)
```
apk install libnss_igshim
```

## Configuration

`libnss_igshim` is configured entirelty within `/etc/nsswitch.conf`.

An example can be found in this repo as [nsswitch.conf](nsswitch.conf) configured for Kanidm

The important bits are defining `igshim` in `initgroups` and `groups` as well as the `#igshim_backend:` line.

## Building locally

```
make clean
make
make install
```
