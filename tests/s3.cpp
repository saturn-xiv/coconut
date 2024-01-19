#include <catch2/catch_test_macros.hpp>

#include "coconut/s3.hpp"
#include "coconut/utils.hpp"

TEST_CASE("Minio", "[minio]") {
  auto config = toml::parse_file("config.toml");
  auto node = config["minio"].as_table();
  const coconut::minio::Config cfg(*node);
  std::cout << "connect " << cfg << std::endl;
  auto cli = cfg.open();

  {
    const auto buckets = cli->list_buckets();
    std::cout << "buckets(" << buckets.size()
              << "): " << coconut::join(buckets, ", ") << std::endl;
  }

  const std::string bucket = "testing";
  {
    // FIXME always return true
    // REQUIRE(!cli->is_bucket_exists(bucket));

    cli->create_bucket(bucket, {{"owner", "Jeremy"}}, false);
    const auto object =
        cli->put_object(bucket, "CMakeDoxyfile.in");  //, {{"Ttl", "3600"}}
    {
      const auto url = cli->get_presigned_object_url(bucket, object);
      std::cout << "7-days(" << object << "): " << url << std::endl;
    }
    {
      const auto objects = cli->list_objects(bucket);
      REQUIRE(objects.size() > 0);
    }
    cli->remove_object(bucket, object);
    cli->remove_bucket(bucket);
  }
}
