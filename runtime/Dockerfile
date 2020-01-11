#  Copyright (c) 2020 Atelier Solutions. All rights reserved.
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.

FROM gcc:latest
WORKDIR /usr/src
RUN git clone https://github.com/lammertb/libhttp.git
WORKDIR /usr/src/libhttp
RUN git checkout bb2577937ae96e19b4c9c3217b5361cd8b7ae591
COPY upstream-patches/libhttp.patch .
RUN patch -p1 < ./libhttp.patch
RUN make

FROM gcc:latest AS knative-c-runtime
LABEL version="1.0"
LABEL author="Chris Baumbauer <cab@atelier-solutions.com>"
LABEL description="Knative C Runtime Library"

WORKDIR /usr/src/knative-c-runtime
COPY . .
COPY --from=0 /usr/src/libhttp/include/libhttp.h .
COPY --from=0 /usr/src/libhttp/lib/libhttp.a .
RUN make lib

RUN mkdir /knative-c-runtime
RUN cp knativeruntime.a /knative-c-runtime/knativeruntime.a
RUN cp knativeruntime.h /knative-c-runtime/knativeruntime.h
RUN cp httpstatus.h /knative-c-runtime/httpstatus.h