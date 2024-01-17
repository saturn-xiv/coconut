/**
 * Autogenerated by Thrift Compiler (0.19.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef auth_TYPES_H
#define auth_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>


namespace coconut { namespace v1 {

struct UserSignInBy {
  enum type {
    EMAIL = 1,
    NICKNAME = 2,
    PHONE = 3,
    GOOGLE_OAUTH2 = 11,
    WECHAT_OAUTH2 = 12,
    WECHAT_MINI_PROGRAM = 13,
    FACEBOOK_OAUTH2 = 14
  };
};

extern const std::map<int, const char*> _UserSignInBy_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const UserSignInBy::type& val);

std::string to_string(const UserSignInBy::type& val);

struct LogLevel {
  enum type {
    DEBUG = 1,
    INFO = 2,
    WARNING = 3,
    ERROR = 4
  };
};

extern const std::map<int, const char*> _LogLevel_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const LogLevel::type& val);

std::string to_string(const LogLevel::type& val);

class Resource;

class Permission;

class UserInfo;

class UserSignInRequest;

class UserSignInResponse;

class UserSignUpRequest;

class LogItem;

typedef struct _Resource__isset {
  _Resource__isset() : type(false), id(false) {}
  bool type :1;
  bool id :1;
} _Resource__isset;

class Resource : public virtual ::apache::thrift::TBase {
 public:

  Resource(const Resource&);
  Resource& operator=(const Resource&);
  Resource() noexcept
           : type(),
             id(0) {
  }

  virtual ~Resource() noexcept;
  std::string type;
  int64_t id;

  _Resource__isset __isset;

  void __set_type(const std::string& val);

  void __set_id(const int64_t val);

  bool operator == (const Resource & rhs) const
  {
    if (!(type == rhs.type))
      return false;
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const Resource &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Resource & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(Resource &a, Resource &b);

std::ostream& operator<<(std::ostream& out, const Resource& obj);

typedef struct _Permission__isset {
  _Permission__isset() : resource(false), operation(false) {}
  bool resource :1;
  bool operation :1;
} _Permission__isset;

class Permission : public virtual ::apache::thrift::TBase {
 public:

  Permission(const Permission&);
  Permission& operator=(const Permission&);
  Permission() noexcept
             : operation() {
  }

  virtual ~Permission() noexcept;
  Resource resource;
  std::string operation;

  _Permission__isset __isset;

  void __set_resource(const Resource& val);

  void __set_operation(const std::string& val);

  bool operator == (const Permission & rhs) const
  {
    if (!(resource == rhs.resource))
      return false;
    if (!(operation == rhs.operation))
      return false;
    return true;
  }
  bool operator != (const Permission &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Permission & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(Permission &a, Permission &b);

std::ostream& operator<<(std::ostream& out, const Permission& obj);

typedef struct _UserInfo__isset {
  _UserInfo__isset() : id(false), nickname(false), email(false), uid(false), real_name(false), avatar(false), current_sign_in_ip(false), current_sign_in_at(false), last_sign_in_ip(false), last_sign_in_at(false), sign_in_total(false), locale(false), timezone(false) {}
  bool id :1;
  bool nickname :1;
  bool email :1;
  bool uid :1;
  bool real_name :1;
  bool avatar :1;
  bool current_sign_in_ip :1;
  bool current_sign_in_at :1;
  bool last_sign_in_ip :1;
  bool last_sign_in_at :1;
  bool sign_in_total :1;
  bool locale :1;
  bool timezone :1;
} _UserInfo__isset;

class UserInfo : public virtual ::apache::thrift::TBase {
 public:

  UserInfo(const UserInfo&);
  UserInfo& operator=(const UserInfo&);
  UserInfo() noexcept
           : id(0),
             nickname(),
             email(),
             uid(),
             real_name(),
             avatar(),
             current_sign_in_ip(),
             current_sign_in_at(0),
             last_sign_in_ip(),
             last_sign_in_at(0),
             sign_in_total(0),
             locale(),
             timezone() {
  }

  virtual ~UserInfo() noexcept;
  int64_t id;
  std::string nickname;
  std::string email;
  std::string uid;
  std::string real_name;
  std::string avatar;
  std::string current_sign_in_ip;
  int64_t current_sign_in_at;
  std::string last_sign_in_ip;
  int64_t last_sign_in_at;
  int64_t sign_in_total;
  std::string locale;
  std::string timezone;

  _UserInfo__isset __isset;

  void __set_id(const int64_t val);

  void __set_nickname(const std::string& val);

  void __set_email(const std::string& val);

  void __set_uid(const std::string& val);

  void __set_real_name(const std::string& val);

  void __set_avatar(const std::string& val);

  void __set_current_sign_in_ip(const std::string& val);

  void __set_current_sign_in_at(const int64_t val);

  void __set_last_sign_in_ip(const std::string& val);

  void __set_last_sign_in_at(const int64_t val);

  void __set_sign_in_total(const int64_t val);

  void __set_locale(const std::string& val);

  void __set_timezone(const std::string& val);

  bool operator == (const UserInfo & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(nickname == rhs.nickname))
      return false;
    if (!(email == rhs.email))
      return false;
    if (!(uid == rhs.uid))
      return false;
    if (!(real_name == rhs.real_name))
      return false;
    if (!(avatar == rhs.avatar))
      return false;
    if (!(current_sign_in_ip == rhs.current_sign_in_ip))
      return false;
    if (!(current_sign_in_at == rhs.current_sign_in_at))
      return false;
    if (!(last_sign_in_ip == rhs.last_sign_in_ip))
      return false;
    if (!(last_sign_in_at == rhs.last_sign_in_at))
      return false;
    if (!(sign_in_total == rhs.sign_in_total))
      return false;
    if (!(locale == rhs.locale))
      return false;
    if (!(timezone == rhs.timezone))
      return false;
    return true;
  }
  bool operator != (const UserInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(UserInfo &a, UserInfo &b);

std::ostream& operator<<(std::ostream& out, const UserInfo& obj);

typedef struct _UserSignInRequest__isset {
  _UserSignInRequest__isset() : code(false), password(false), ttl(true) {}
  bool code :1;
  bool password :1;
  bool ttl :1;
} _UserSignInRequest__isset;

class UserSignInRequest : public virtual ::apache::thrift::TBase {
 public:

  UserSignInRequest(const UserSignInRequest&);
  UserSignInRequest& operator=(const UserSignInRequest&);
  UserSignInRequest() noexcept
                    : code(),
                      password(),
                      ttl(86400LL) {
  }

  virtual ~UserSignInRequest() noexcept;
  std::string code;
  std::string password;
  int64_t ttl;

  _UserSignInRequest__isset __isset;

  void __set_code(const std::string& val);

  void __set_password(const std::string& val);

  void __set_ttl(const int64_t val);

  bool operator == (const UserSignInRequest & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(password == rhs.password))
      return false;
    if (!(ttl == rhs.ttl))
      return false;
    return true;
  }
  bool operator != (const UserSignInRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserSignInRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(UserSignInRequest &a, UserSignInRequest &b);

std::ostream& operator<<(std::ostream& out, const UserSignInRequest& obj);

typedef struct _UserSignInResponse__isset {
  _UserSignInResponse__isset() : real_name(false), token(false), roles(true), permissions(true), locale(false), timezone(false), by(false) {}
  bool real_name :1;
  bool token :1;
  bool roles :1;
  bool permissions :1;
  bool locale :1;
  bool timezone :1;
  bool by :1;
} _UserSignInResponse__isset;

class UserSignInResponse : public virtual ::apache::thrift::TBase {
 public:

  UserSignInResponse(const UserSignInResponse&);
  UserSignInResponse& operator=(const UserSignInResponse&);
  UserSignInResponse() noexcept
                     : real_name(),
                       token(),
                       locale(),
                       timezone(),
                       by(static_cast<UserSignInBy::type>(0)) {


  }

  virtual ~UserSignInResponse() noexcept;
  std::string real_name;
  std::string token;
  std::vector<std::string>  roles;
  std::vector<Permission>  permissions;
  std::string locale;
  std::string timezone;
  /**
   * 
   * @see UserSignInBy
   */
  UserSignInBy::type by;

  _UserSignInResponse__isset __isset;

  void __set_real_name(const std::string& val);

  void __set_token(const std::string& val);

  void __set_roles(const std::vector<std::string> & val);

  void __set_permissions(const std::vector<Permission> & val);

  void __set_locale(const std::string& val);

  void __set_timezone(const std::string& val);

  void __set_by(const UserSignInBy::type val);

  bool operator == (const UserSignInResponse & rhs) const
  {
    if (!(real_name == rhs.real_name))
      return false;
    if (!(token == rhs.token))
      return false;
    if (!(roles == rhs.roles))
      return false;
    if (!(permissions == rhs.permissions))
      return false;
    if (!(locale == rhs.locale))
      return false;
    if (!(timezone == rhs.timezone))
      return false;
    if (!(by == rhs.by))
      return false;
    return true;
  }
  bool operator != (const UserSignInResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserSignInResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(UserSignInResponse &a, UserSignInResponse &b);

std::ostream& operator<<(std::ostream& out, const UserSignInResponse& obj);

typedef struct _UserSignUpRequest__isset {
  _UserSignUpRequest__isset() : real_name(false), nickname(false), email(false), password(false), locale(false), timezone(false) {}
  bool real_name :1;
  bool nickname :1;
  bool email :1;
  bool password :1;
  bool locale :1;
  bool timezone :1;
} _UserSignUpRequest__isset;

class UserSignUpRequest : public virtual ::apache::thrift::TBase {
 public:

  UserSignUpRequest(const UserSignUpRequest&);
  UserSignUpRequest& operator=(const UserSignUpRequest&);
  UserSignUpRequest() noexcept
                    : real_name(),
                      nickname(),
                      email(),
                      password(),
                      locale(),
                      timezone() {
  }

  virtual ~UserSignUpRequest() noexcept;
  std::string real_name;
  std::string nickname;
  std::string email;
  std::string password;
  std::string locale;
  std::string timezone;

  _UserSignUpRequest__isset __isset;

  void __set_real_name(const std::string& val);

  void __set_nickname(const std::string& val);

  void __set_email(const std::string& val);

  void __set_password(const std::string& val);

  void __set_locale(const std::string& val);

  void __set_timezone(const std::string& val);

  bool operator == (const UserSignUpRequest & rhs) const
  {
    if (!(real_name == rhs.real_name))
      return false;
    if (!(nickname == rhs.nickname))
      return false;
    if (!(email == rhs.email))
      return false;
    if (!(password == rhs.password))
      return false;
    if (!(locale == rhs.locale))
      return false;
    if (!(timezone == rhs.timezone))
      return false;
    return true;
  }
  bool operator != (const UserSignUpRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserSignUpRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(UserSignUpRequest &a, UserSignUpRequest &b);

std::ostream& operator<<(std::ostream& out, const UserSignUpRequest& obj);

typedef struct _LogItem__isset {
  _LogItem__isset() : id(false), user_id(false), plugin(false), message(false), ip(false), level(false), created_at(false) {}
  bool id :1;
  bool user_id :1;
  bool plugin :1;
  bool message :1;
  bool ip :1;
  bool level :1;
  bool created_at :1;
} _LogItem__isset;

class LogItem : public virtual ::apache::thrift::TBase {
 public:

  LogItem(const LogItem&);
  LogItem& operator=(const LogItem&);
  LogItem() noexcept
          : id(0),
            user_id(0),
            plugin(),
            message(),
            ip(),
            level(static_cast<LogLevel::type>(0)),
            created_at() {
  }

  virtual ~LogItem() noexcept;
  int64_t id;
  int64_t user_id;
  std::string plugin;
  std::string message;
  std::string ip;
  /**
   * 
   * @see LogLevel
   */
  LogLevel::type level;
  std::string created_at;

  _LogItem__isset __isset;

  void __set_id(const int64_t val);

  void __set_user_id(const int64_t val);

  void __set_plugin(const std::string& val);

  void __set_message(const std::string& val);

  void __set_ip(const std::string& val);

  void __set_level(const LogLevel::type val);

  void __set_created_at(const std::string& val);

  bool operator == (const LogItem & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    if (!(plugin == rhs.plugin))
      return false;
    if (!(message == rhs.message))
      return false;
    if (!(ip == rhs.ip))
      return false;
    if (!(level == rhs.level))
      return false;
    if (!(created_at == rhs.created_at))
      return false;
    return true;
  }
  bool operator != (const LogItem &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LogItem & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot) override;
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const override;

  virtual void printTo(std::ostream& out) const;
};

void swap(LogItem &a, LogItem &b);

std::ostream& operator<<(std::ostream& out, const LogItem& obj);

}} // namespace

#endif
