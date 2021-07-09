1. docker build -t example .
2. xhost +
3. docker run -e DISPLAY=unix$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix example
4. xhost -
