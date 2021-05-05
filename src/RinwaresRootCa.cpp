/*****************************************************************************
 *
 *  PROJECT:            RakCloudDesktop
 *  LICENSE:            See LICENSE in the top level directory
 *  FILE:               RinwaresRootCa.cpp
 *  DESCRIPTION:        RINWARES ROOT CA certificate
 *  COPYRIGHT:          (c) 2021 RINWARES <rinwares.com>
 *  AUTHOR:             Rinat Namazov <rinat.namazov@rinwares.com>
 *
 *****************************************************************************/

#include <QSslCertificate>
#include <QSslConfiguration>

void addRinwaresRootCaToDefaultConfiguration() {
    const QSslCertificate rinwaresRootCaCertificate(R"(-----BEGIN CERTIFICATE-----
MIIFnzCCA4egAwIBAgIUAIXH/3Ejp1uPRnYJgNL+5rr2XZEwDQYJKoZIhvcNAQEL
BQAwVzERMA8GA1UECgwIUklOV0FSRVMxJzAlBgNVBAsMHlJJTldBUkVTIENlcnRp
ZmljYXRlIEF1dGhvcml0eTEZMBcGA1UEAwwQUklOV0FSRVMgUm9vdCBDQTAeFw0y
MTA1MDMxODMyMTRaFw00MTA0MjgxODMyMTRaMFcxETAPBgNVBAoMCFJJTldBUkVT
MScwJQYDVQQLDB5SSU5XQVJFUyBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkxGTAXBgNV
BAMMEFJJTldBUkVTIFJvb3QgQ0EwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK
AoICAQCqYPIZKYOXIR42aSOLO6sGuyyz8/qrBKufgknJSJBRasecC656+DzBFH5/
YmZQNGDz7x4Zd6Efa6flstCjNMWCbvR7dwKyYjhcP5IqSk3OnoCqJ2kgDt5oiH6H
8fe9L4/zJCw5hYTxrs29mfivSFZec01qPEtNg1C2li/NUEfvYXU8XQYI6SjZ6///
mdG2vtr+5XaKwI9JY4wzkvsKWXKSig4C21MhuKuTzlqm8INaYcOmu7O2+JFC9NyI
EOQIPZwVnbChSbwV1AiGVarNy7HaOpkbsQcnAfKViSTA6Uioccnhr/oInIHpC6BL
GtPXshr8vlkp0lVdopmxF4sEGmkHe5LtYkk8fAJYeBCATo8q0MKnpo4KKz+kuXzi
kOClIOEoA1RK59vGxnNPDm2PhjXYsHJl30i55eXASeBuEu7xArZYUXp2RVbFUk6R
Pk68XUIPBLDqLKBcjiC//t32MMe8y+119iu/5FnM3zSmAMZHe+2nxYVUucPmcl/v
AupPghRdj0HFVl03RaMnAPGrPA4YYTL2/yqikRf33juLLqI4caU4gUe5/M4C4cKk
GOo+mPnUvqmjLZg5NKzX5ZovXNigzjXCtywPsC7IAwbud5r1eu94bxZMQsx6W/mY
FxcfjS5+uXvm+Cc63DnWUQ97qy3EHZXlJbhglxnuBLgVfxXHSQIDAQABo2MwYTAd
BgNVHQ4EFgQUjTS1x5PdYc8u3vkubsSvh4c/vIIwHwYDVR0jBBgwFoAUjTS1x5Pd
Yc8u3vkubsSvh4c/vIIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYw
DQYJKoZIhvcNAQELBQADggIBAJxPLrdKxSwgych1sPvqmYZQhUoMQdur+EhzdGWB
RGqjP8VrR0Uyljru5jjqgqgjAQ+ngUTZC/SjiHoYU19OBVm0B8uOUmLPOJnyyiKU
8+AWoSwDS8rZmhmYUDbTKYFjCKQoqteIMC/P0EblSItYfP8jrbH7gO2zKUTDl2Zq
XTMCijDDv2NIxBxbIkJQWc94m3qv17Dk7SWTDSpdI+zZ/sLo1A3WQI/vSajB1bbh
Lsg0RtiwBx/b5qXdrm120a5g4G7GFf2K3d0BZJ6yd6CIMPboSAj9hwUXxgYTszn5
TEiagvtOacQAjSW//4WrY5DsevLXL/97rnVGQkn6HumQbCLDD9ugdC3Nu488fFdr
Sos5zB8pPsOW3S9L4YZrc2iOMos0qyrD0DQmjY0eXBzj+t7x93GR3DVe1GfMaHm6
yCS8CbH+HMLfZadzwJLoqgt7xEAjBafiVH71BMc3XSMFIsDLHbm12MaqpeDZfgE2
juVbo2aMPzWAoVApIiJ7kTeWJkV5s1dE3dEHp4Y6d41q35XJjScA0RtIDTaxWvg1
vBojCq1vfZ39yPiBsDV1A6Wq9Fry1YSEUio4FPxSM3GzCqENLm7V2CxBUshE5gv0
1vDPeACE2uD/oxa7bCWF7H7eSbPQOtjVwHJ2zFfbwZK7JxC5SoMToTd0GjUB5r02
Arzz
-----END CERTIFICATE-----)");

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.addCaCertificate(rinwaresRootCaCertificate);
    QSslConfiguration::setDefaultConfiguration(sslConfig);
}
