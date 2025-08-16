FROM ubuntu:latest

WORKDIR /app

COPY src/server.cpp .
COPY src/controllers/ controllers/

RUN apt-get update && \
    apt-get install -y build-essential && \
    g++ -c server.cpp -o server.o && \
    g++ -c controllers/health_controller.cpp -o controllers/health_controller.o && \
    g++ -c controllers/metrics_controller.cpp -o controllers/metrics_controller.o && \
    g++ server.o controllers/health_controller.o controllers/metrics_controller.o -o server

EXPOSE 8080

CMD ["./server"]
