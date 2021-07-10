1.ss2019.Dockerfile
1.1. docker build -t ss2019 .
///////////////////////////////////
2. Dockerfile
2.1. docker build -t example .
2.2. xhost +
2.3. docker run -e DISPLAY=unix$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix example
2.4. xhost -
