#!/bin/bash

# Set the source and destination directories
src_dir="./libPS4"
dest_dir="/opt/ps4sdk"

# Check if the destination directory exists
if [ -d "$dest_dir" ]; then
  # Prompt the user before overwriting any files
  read -p "The directory $dest_dir already exists. Do you want to delete it and recreate it? (y/n) " -n 1 -r
  echo
  if [[ $REPLY =~ ^[Yy]$ ]]; then
    rm -rf "$dest_dir"
    mkdir -p "$dest_dir"
  else
    exit 1
  fi
else
  mkdir -p "$dest_dir"
fi

# Build SDK
cd "$src_dir" || (echo "Unable to enter subdirectory $src_dir" && exit 1)
make
cd -

# Copy compiled SDK
cp -r "$src_dir" "$dest_dir"
cp update.sh "$dest_dir/update.sh"

# Clear path from .bashrc if it's there
sed -i "s/^\s*export PS4SDK=.*//gm" /etc/profile

# Add to paths
#echo "export PS4SDK=$dest_dir" | tee -a /etc/profile
export PS4SDK="$dest_dir"
echo "done"
