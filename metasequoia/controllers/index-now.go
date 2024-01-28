package controllers

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func IndexNowSiteVerify() gin.HandlerFunc {
	return func(c *gin.Context) {
		// TODO
		c.HTML(http.StatusOK, "index-now/site-verify.html", gin.H{
			"title": "Hello palm",
		})
	}
}
