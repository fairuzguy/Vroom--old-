#!/bin/bash

cp template.html out/index.html

pushd out
	python -m SimpleHTTPServer 8000 & open http://localhost:8000
popd
