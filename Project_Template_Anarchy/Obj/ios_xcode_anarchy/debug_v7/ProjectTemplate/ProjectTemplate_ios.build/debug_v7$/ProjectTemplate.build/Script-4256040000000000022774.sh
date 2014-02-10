#!/bin/sh
python $VISION_SDK/Build/StandaloneTools/Deployment/Python/copyiOSResources.py -d $BUILT_PRODUCTS_DIR/$CONTENTS_FOLDER_PATH -r Data -r Assets
