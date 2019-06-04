#!/bin/bash

version=10000

rebuild_flg=1

has_version_record=0

build_configuration=$1

cd `dirname $0`/.

build_action()
{
	if [ -f HFStaticAnalysis.version ]; then
		for line in `cat HFStaticAnalysis.version`; do
			if [[ "$line" =~ ^version=[0-9]{1,} ]]; then
				ori_version=$(echo $line | sed 's/^version=\([0-9]\{1,\}\)/\1/g')

				has_version_record=1

				echo -e "\033[35m[HFStaticAnalysis]\033[0m local HFStaticAnalysis.dylib version: \033[31mv$ori_version\033[0m"

				if (( $version <= $ori_version )); then
					rebuild_flg=0
				fi
				break
			fi
		done
	else
		echo -e "\033[35m[HFStaticAnalysis]\033[0m Not installed HFStaticAnalysis.dylib"
	fi

	if [ 1 == $has_version_record ]; then
		sed -i "s/^version=[0-9]\{1,\}/version=$version/g" HFStaticAnalysis.version
	else
		echo "version=$version" >> HFStaticAnalysis.version
	fi

	if [ 1 == $rebuild_flg ]; then
		echo -e "\033[35m[HFStaticAnalysis]\033[0m upgrade HFStaticAnalysis.dylib to version: \033[31mv$version\033[0m ..."
		xcodebuild -project HFStaticAnalysis.xcodeproj -target HFStaticAnalysis -configuration Release && rm -rf build/HFStaticAnalysis.build && rm -rf build/XCBuildData
	fi
}

if [ -z $build_configuration ] || [ Debug == $build_configuration ]; then
	build_action
fi

