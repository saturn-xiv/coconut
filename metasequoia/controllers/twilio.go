package controllers

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func TwilioMessagingStatusCallback() gin.HandlerFunc {
	return func(c *gin.Context) {
		// TODO
		c.JSON(http.StatusOK, gin.H{
			"title": "Hello palm",
		})
	}
}
