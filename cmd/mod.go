package cmd

import (
	"fmt"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
)

var (
	git_version  string
	author_email string
	author_name  string
	repo_url     string
	build_time   string
)

var root_cmd = &cobra.Command{
	Use:     "coconut",
	Short:   "Coconut",
	Long:    fmt.Sprintf("A flexible authentication solution by Go.(%s).", repo_url),
	Version: fmt.Sprintf("%s(%s) by %s<%s>", git_version, build_time, author_name, author_email),
	Run: func(cmd *cobra.Command, args []string) {
		if err := cmd.Help(); err != nil {
			log.Fatal(err)
		}
	},
}

func Execute() {
	if err := root_cmd.Execute(); err != nil {
		log.Fatal(err)
	}
}

var (
	gl_debug        bool
	gl_config       string
	gl_rpc_port     int
	gl_web_port     int
	gl_worker_queue string
)

func init() {
	root_cmd.PersistentFlags().BoolVarP(&gl_debug, "debug", "d", false, "run on debug mode")
	root_cmd.PersistentFlags().StringVarP(&gl_config, "config", "c", "config.toml", "load configuration file")

	{
		var cmd = &cobra.Command{
			Use:   "dump",
			Short: "Dump config scripts",
			Run: func(cmd *cobra.Command, args []string) {
				if gl_debug {
					log.SetLevel(log.DebugLevel)
				} else {
					log.SetLevel(log.InfoLevel)
				}

				log.Debugf("run on debug mode")
				log.Debugf("load configuration from %s", gl_config)
				// config, err := env.NewEnvironment(gl_config)
				// if err != nil {
				// 	log.Fatalf("parse file: %s", err)
				// }
				// TODO
				// if err := launch_dump(config); err != nil {
				//         log.Fatal(err)
				//         return
				// }
			},
		}
		root_cmd.AddCommand(cmd)
	}

	{
		var cmd = &cobra.Command{
			Use:   "web",
			Short: "Start a HTTP server",
			Run: func(cmd *cobra.Command, args []string) {
				if gl_debug {
					log.SetLevel(log.DebugLevel)
				} else {
					log.SetLevel(log.InfoLevel)
				}

				log.Debugf("run on debug mode")
				log.Debugf("load configuration from %s", gl_config)
				// config, err := env.NewRpc(gl_config)
				// if err != nil {
				// 	log.Fatalf("parse file: %s", err)
				// }

				// TODO
				// if err := launch_rpc_server(config, gl_rpc_port); err != nil {
				//         log.Fatalf("start gRPC server: %s", err)
				// }
			},
		}
		cmd.Flags().IntVarP(&gl_web_port, "port", "p", 8080, "listen port")
		root_cmd.AddCommand(cmd)
	}

	{
		var cmd = &cobra.Command{
			Use:   "rpc",
			Short: "Start a gRPC server",
			Run: func(cmd *cobra.Command, args []string) {
				if gl_debug {
					log.SetLevel(log.DebugLevel)
				} else {
					log.SetLevel(log.InfoLevel)
				}

				log.Debugf("run on debug mode")
				log.Debugf("load configuration from %s", gl_config)
				// config, err := env.NewRpc(gl_config)
				// if err != nil {
				// 	log.Fatalf("parse file: %s", err)
				// }

				// TODO
				// if err := launch_rpc_server(config, gl_rpc_port); err != nil {
				//         log.Fatalf("start gRPC server: %s", err)
				// }
			},
		}
		cmd.Flags().IntVarP(&gl_rpc_port, "port", "p", 8080, "listen port")
		root_cmd.AddCommand(cmd)
	}

	{
		var cmd = &cobra.Command{
			Use:   "worker",
			Short: "Start a queue consumer",
			Run: func(cmd *cobra.Command, args []string) {

				log.Debugf("load configuration from %s", gl_config)
				// config, err := env.NewRpc(gl_config)
				// if err != nil {
				// 	log.Fatalf("parse file: %s", err)
				// }

				// TODO
				// if err := launch_rpc_server(config, gl_rpc_port); err != nil {
				//         log.Fatalf("start gRPC server: %s", err)
				// }
			},
		}
		cmd.Flags().StringVar(&gl_worker_queue, "queue", "q", "queue name")
		root_cmd.AddCommand(cmd)
	}

}

func set_log_level() {
	if gl_debug {
		log.SetLevel(log.DebugLevel)
	} else {
		log.SetLevel(log.InfoLevel)
	}

	log.Debugf("run on debug mode")
}
