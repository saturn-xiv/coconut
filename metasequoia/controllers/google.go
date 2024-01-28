package controllers

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func GoogleSiteVerify() gin.HandlerFunc {
	return func(c *gin.Context) {
		// TODO
		c.HTML(http.StatusOK, "google/site-verify.html", gin.H{
			"title": "Hello palm",
		})
	}
}
