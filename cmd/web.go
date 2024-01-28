package cmd

import (
	"embed"
	"fmt"
	"html/template"
	"net/http"

	"github.com/gin-gonic/gin"
	log "github.com/sirupsen/logrus"

	"github.com/saturn-xiv/coconut/env"
	"github.com/saturn-xiv/coconut/metasequoia/controllers"
)

//go:embed assets/*
var gl_assets_fs embed.FS

//go:embed templates/*
var gl_templates_fs embed.FS

func launch_http_server(port int, config *env.Config) error {
	address := fmt.Sprintf("0.0.0.0:%d", port)
	router := gin.Default()

	tpl := template.Must(template.New("").ParseFS(gl_templates_fs, "templates/*/*.html"))
	router.SetHTMLTemplate(tpl)
	router.StaticFS("/public", http.FS(gl_assets_fs))

	router.GET("/robots.txt", controllers.RobotsTxt())
	router.GET("/sitemap.xml", controllers.SitemapXml())
	router.GET("/:lang/sitemap.xml", controllers.SitemapXmlByLang())
	router.GET("/:lang/rss.xml", controllers.RssXml())
	router.GET("/google.html", controllers.GoogleSiteVerify())
	router.GET("/baidu.html", controllers.BaiduSiteVerify())
	router.GET("/index-now.html", controllers.IndexNowSiteVerify())

	log.Infof("listen on http://%s", address)
	return router.Run(address)
}
