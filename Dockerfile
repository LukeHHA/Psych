FROM ubuntu:latest AS build

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libboost-dev \
    libgl1-mesa-dev \
    libwayland-bin \
    libwayland-dev \
    libxkbcommon-dev \
    ninja-build \
    pkg-config \
    python3 \
    zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src 

RUN mkdir -p /build

COPY . .

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DPSYCH_ENABLE_ASSERTS=OFF -DPSYCH_ENABLE_PROFILING=OFF \
    -DPSYCH_ENGINE_BUILD_EDITOR=ON 

RUN cmake --build /src/build --target Editor --parallel 

FROM ubuntu:latest AS runner 

WORKDIR /src 

COPY --from=build /src/out/Editor ./

CMD ["./Editor", "--data-dir", "."]


