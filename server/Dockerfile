FROM ubuntu:latest

WORKDIR /app

COPY src/server.cpp .
COPY src/controllers/ controllers/
COPY src/utils/ utils/

RUN apt-get update && \
    apt-get install -y build-essential && \
    mkdir -p /var/log/server && \
    g++ -c server.cpp -o server.o -std=c++11 -pthread && \
    g++ -c utils/httprequest_parser.cpp -o httprequest_parser.o -std=c++11 -pthread && \
    g++ -c utils/logger.cpp -o logger.o -std=c++11 -pthread && \
    g++ -c controllers/health_controller.cpp -o health_controller.o -std=c++11 -pthread && \
    g++ -c controllers/metrics_controller.cpp -o metrics_controller.o -std=c++11 -pthread && \
    g++ -c controllers/hello_controller.cpp -o hello_controller.o -std=c++11 -pthread && \
    g++ -c controllers/bye_controller.cpp -o bye_controller.o -std=c++11 -pthread && \
    g++ server.o httprequest_parser.o logger.o health_controller.o metrics_controller.o hello_controller.o bye_controller.o -o server -std=c++11 -pthread

EXPOSE 8080

CMD ["./server"]
