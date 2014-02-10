#!/bin/sh
python $VISION_SDK/Build/StandaloneTools/Deployment/Python/linkiOSResources.py -r ../../Data/Vision/Base/ Data/Vision/Base/ -r ../../Assets/ Assets/ -l $CONFIGURATION
