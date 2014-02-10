#!/bin/sh

mkdir -p ./Data/Vision
cp -R -f "$VISION_SDK/Data/Vision/Base" ./Data/Vision

mkdir -p ./Data/Common/iOS
cp -R -f "$VISION_SDK/Data/Common/iOS" ./Data/Common

mkdir -p ./Data/Common/Anarchy
cp -R -f "$VISION_SDK/Data/Common/Anarchy" ./Data/Common

mkdir -p ./Source/GameApplication/Common/iOS
cp -R -f "$VISION_SDK/Source/Vision/Runtime/Common/iOS" ./Source/GameApplication/Common