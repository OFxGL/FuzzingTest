FROM gcr.io/oss-fuzz-base/base-builder:v1
COPY . $SRC/fuzz
WORKDIR $SRC/fuzz
COPY ./.clusterfuzzlite/build.sh $SRC/