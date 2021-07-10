FROM ss2019
RUN apt-get update && apt-get upgrade
COPY ./hm31.cpp /usr/src
COPY ./sample.MOV /usr/src
WORKDIR /usr/src
RUN g++ hm31.cpp -o output `pkg-config --cflags --libs opencv` && \
echo /usr/local/lib>/etc/ld.so.conf.d/opencv.conf && \
ldconfig -v
CMD ./output
