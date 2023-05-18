# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
# Base image for compiling C programs
FROM alpine:latest

RUN echo "https://dl-4.alpinelinux.org/alpine/edge/community" >> /etc/apk/repositories;
RUN echo "https://dl-4.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories;

RUN apk update && apk add make g++ bash python3 swig python3-dev

# These commands copy your files into the specified directory in the image
# and set that as the working location
# VOLUME /molviewerapp

# Copy all files to src_molviewer directory
COPY . /usr/src/src_molviewer 

# Set the working directory to src_molviewer
WORKDIR /usr/src/src_molviewer/molviewerapp/src


# Run the makefile in the src directory
CMD ["bash", "-c", "make && bash"]

# Set the entrypoint to an empty array
ENTRYPOINT []

# LABEL Name=srcmolviewer Version=0.0.1

        # docker build . -t molviewerapp:fullstack
        # docker run --rm --name molviewer-container -p 8000:8000 -v $PWD/molviewerapp:/src_molviewer -it molviewerapp:fullstack
