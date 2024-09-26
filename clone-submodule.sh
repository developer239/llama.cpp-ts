#!/bin/bash

# Define the directory where the submodule should be cloned
SUBMODULE_DIR="cpp/externals/llama-chat"

# Check if the directory already exists
if [ ! -d "$SUBMODULE_DIR" ]; then
  # Clone the submodule if it doesn't exist
  git clone --recurse-submodules https://github.com/developer239/llama-chat.git $SUBMODULE_DIR
else
  echo "Submodule already exists, pulling latest changes..."
  git -C $SUBMODULE_DIR pull
fi
