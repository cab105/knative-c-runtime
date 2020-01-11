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

FROM cab105/knative-c-runtime AS serverless-function
WORKDIR /usr/src/serverless-function
COPY --from=cab105/knative-c-runtime /knative-c-runtime/knativeruntime.a .
COPY --from=cab105/knative-c-runtime /knative-c-runtime/httpstatus.h .
COPY --from=cab105/knative-c-runtime /knative-c-runtime/knativeruntime.h .
COPY . /usr/src/serverless-function
RUN make app

FROM debian:latest
COPY --from=serverless-function /usr/src/serverless-function/app .
EXPOSE 8080/tcp
CMD ["./app"]