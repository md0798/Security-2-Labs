Name - Meet Desai
UNI- mpd2155

Problem 2:

Part A:

In part A we had to make root certificate, intermediate certificate, server certificate and client certificate. First I start with making some directories to store all the certificates and private keys, then I start by making a private key for root certificate with passphrase 0000, after which I create the root certificate the fields can be leaved blank while making it. Then, I create the intermediate certificate with passphrase 0000 using the same procedure (Note: the commonName should be mpd), after that I sign the intermediate certificate using the root certificate (type y 2 times when prompted), then verify it and then create a chain with both root and intermediate certificate. Then I create client certificate using the same procedure ,i.e., creating key first then creating the certificate(commonName should be mpd2155@columbia.edu), I use the intermediate certificate to sign the client certificate(type y 2 times when prompted) and verify the certificate and then verify it against the chain certificate and then I create the server certificate using the same procedure (commonName should be www.mpd2155.com) then sign the server certificate using intermediate certificate(type y 2 times when prompted) and verify the certificate and then verify it against the chain certificate.


Part B:

In part B I have two separate scripts one for server other for client. 
The server script first makes a server directory then copies the servers key, server certificate and chain certificate into the directory. I then create a test.txt file inside the server folder and fill it with my name and then sign it using openssl dgst function and then start the server using openssl s_server at port 8443 in -HTTP mode.

The client script makes a client directory and then copies the client's key, server certificate and chain certificate into the directory. I then request the test.txt file using HTTP GET from the server, I request the test.sign file using HTTP GET from the server, I request the public key of the server to verify the signature and then I verify the public key of server against the test.sign using the test.txt file with the help of openssl dgst function.


Execution:

The files submitted are p1,p2_c,p2_s,Problem2. Open a terminal and execute

$bash Problem2

This will execute script p1 followed by p2_s.
Open another terminal and execute

$bash p2_c 

