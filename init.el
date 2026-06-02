;; hide the default splash screen on startup
(setq inhibit-startup-message t)

;; disable visible UI clutter
(menu-bar-mode -1)
(tool-bar-mode -1)
(scroll-bar-mode -1)

;; highlight the current line
(global-hl-line-mode 1)
;; display line numbers in buffers
(global-display-line-numbers-mode 1)
(column-number-mode t)

;; turn off cursor blinking
(blink-cursor-mode 0)

;; show date
(setq display-time-day-and-date t)
(display-time-mode 1)

;; font
(set-frame-font "Fira Code-16" nil t)

;; enable interactive autocompletion pairings (brackets, quotes)
(electric-pair-mode 1)
;; remember the last cursor position when reopening a file
(save-place-mode 1)

;; start with maximized frame
(add-hook 'window-setup-hook 'toggle-frame-maximized t)

;; ---------- solarized-theme ----------
(load-theme 'solarized-dark t)
;; (load-theme 'solarized-light t)
;; ------------------------------------

