#!/usr/bin/env bash

##--------------------------------------------------------------------
## Copyright (c) 2019 Dianomic Systems
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##--------------------------------------------------------------------

##
## Author: Mark Riddoch, Massimiliano Pinto
##

set -e

fledge_location=`pwd`
os_name=`(grep -o '^NAME=.*' /etc/os-release | cut -f2 -d\" | sed 's/"//g')`
os_version=`(grep -o '^VERSION_ID=.*' /etc/os-release | cut -f2 -d\" | sed 's/"//g')`
echo "Platform is ${os_name}, Version: ${os_version}"

if [[ ( $os_name == *"Red Hat"* || $os_name == *"CentOS"* ) &&  $os_version == *"7"* ]]; then
	sudo yum -y install curl
	sudo yum -y install libcurl
	sudo yum -y install curl-devel
	sudo yum -y install libcurl-devel
elif apt --version 2>/dev/null; then
	sudo apt -y install libcurl4-openssl-dev
else
	echo "Requirements cannot be automatically installed, please refer README.rst to install requirements manually"
fi
