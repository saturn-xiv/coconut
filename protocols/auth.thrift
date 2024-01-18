namespace cpp coconut.v1
namespace java com.github.saturn_xiv.coconut.v1

const string ROLE_ADMINISTRATOR = "administrator";
const string ROLE_ROOT = "root";
const string UNKNOW_IP = "n/a";

struct Resource {
    1:string type;
    2:i64 id;
}

struct Permission {
    1:Resource resource;
    2:string operation; 
}

struct UserInfo {
    1:i64 id;
    2:string nickname;
    3:string email;
    4:string uid;

    11:string real_name;
    12:string avatar;
    
    85:string current_sign_in_ip;
    86:i64 current_sign_in_at;
    87:string last_sign_in_ip;
    88:i64 last_sign_in_at;
    89:i64 sign_in_total;
    
    98:string locale;
    99:string timezone;
}

# -----------------------------------------------------------------------------

enum UserSignInBy {
    EMAIL = 1;
    NICKNAME = 2;
    PHONE = 3;
    GOOGLE_OAUTH2 = 11;
    WECHAT_OAUTH2 = 12;
    WECHAT_MINI_PROGRAM = 13;
    FACEBOOK_OAUTH2 = 14;
}

struct UserSignInRequest {
    1:string code;
    2:string password;
    3:i64 ttl=86400; 
}

struct UserSignInResponse {
    1:string real_name;
    2:string token;

    11:list<string> roles=[];
    12:list<Permission> permissions=[];

    97:string locale;
    98:string timezone;
    99:UserSignInBy by;
}

struct UserSignUpRequest {
    1:string real_name;
    2:string nickname;
    3:string email;
    4:string password;
    5:string locale;
    6:string timezone;
}

service User {
    UserSignInResponse sign_in(1:UserSignInRequest request);
    void sign_up(1:UserSignUpRequest request);
    void confirm_by_email(1:string email);
    void configm_by_token(1:string token);
    void unlock_by_email(1:string email);
    void unlock_by_token(1:string token);
    void forgot_password(1:string email);
    void reset_password(1:string token, 2:string password);
    
    void set_real_name(1:i64 id, 2:string real_name);
    void set_avatar(1:i64 id, 2:string avatar);
    void set_locale(1:i64 id, 2:string locale);
    void set_timezone(1:i64 id, 2:string timezone);
    void set_password(1:i64 id, 2:string password);

    void set_(1:i64 id, 2:string key, 3:binary value);
    binary get_(1:i64 id, 2:string key);

    void confirm(1:i64 id);
    void lock(1:i64 id);
    void unlock(1:i64 id);
    void enable(1:i64 id);
    void disable(1:i64 id);

    list<UserInfo> all();
    UserInfo by_nickname(1:string nickname);
    UserInfo by_email(1:string email);
    UserInfo by_id(1:i64 id);
    UserInfo by_uid(1:string uid);
}

# -----------------------------------------------------------------------------

enum LogLevel {
    DEBUG = 1;
    INFO = 2;
    WARNING = 3;
    ERROR = 4;
}

struct LogItem {
    1:i64 id;
    2:i64 user_id;

    3:string plugin;
    4:string message;
    5:string ip;
    6:LogLevel level;

    9:string created_at;
}

service Log {
    list<LogItem> by_user(1:i64 user);
    list<LogItem> by_year_and_month(1:i16 year, 2:i8 month);
}


# -----------------------------------------------------------------------------

service Policy {
    void can(1:i64 user, 2:string operation, 3:Resource resource);
    void has(1:i64 user, 2:string role);
    void add_roles_for_user(1:i64 user, 2:list<string> roles);
    void remove_roles_for_user(1:i64 user, 2:list<string> roles);
    list<string> get_roles_for_user(1:i64 user);
    list<i64> get_users_for_role(1:string role);
    
    void add_permissions_for_user(1:i64 user, 2:list<Permission> permissions);
    void remove_permissions_for_user(1:i64 user, 2:list<Permission> permissions);
    void add_permissions_for_role(1:string role, 2:list<Permission> permissions);
    void remove_permissions_for_role(1:string role, 2:list<Permission> permissions);
    list<Permission> get_permissions_for_user(1:i64 user);
    list<Permission> get_permissions_for_role(1:string role);
}
