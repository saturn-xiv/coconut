package controllers

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func BaiduSiteVerify() gin.HandlerFunc {
	return func(c *gin.Context) {
		// TODO
		c.HTML(http.StatusOK, "baidu/site-verify.html", gin.H{
			"title": "Hello palm",
		})
	}
}
