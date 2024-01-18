namespace cpp coconut.v1
namespace java com.github.saturn_xiv.coconut.v1

struct JwtVerfifyResponse {
    1:string subject,
    2:string jwt_id,
    3:string payload,
}

service Jwt {
    string sign(1:string issuer, 2:string subject, 3:string audience, 4:i64 ttl, 5: string payload);
    JwtVerfifyResponse verify(1:string token, 2:string issuer, 3:string audience);
}

service Hmac {
    binary sign(1:binary plain);
    void verify(1:binary code, 2:binary plain);
}

struct AesEncryptResponse {
    1:binary code,
    2:binary salt,
}

service Aes {
    AesEncryptResponse encrypt(1:binary plain);
    binary decrypt(1:binary code, 2:binary salt);
}
