FROM debian:bookworm

LABEL author="punkertron"

RUN apt-get update && \
	apt-get install -y && \
		g++ \
		make \
		binutils \
		cmake \
		libboost-system-dev \
		libssl-dev \
		zlib1g-dev \
		libcurl4-openssl-dev \
		git \
		curl && \
	apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /telegram_bot

COPY incs incs
COPY srcs srcs
COPY Makefile .

RUN git clone https://github.com/reo7sp/tgbot-cpp.git && \
	cd tgbot-cpp && \
	cmake . && \
    make -j$(nproc) && \
    make install && \
	cd .. && \
    rm -rf /tgbot-cpp && \
	make

ENTRYPOINT ["./telegram_bot"]