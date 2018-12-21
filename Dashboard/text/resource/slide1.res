(Layout slide1
    (Components
		(SubLayout                      Layout1)
    )
	
	(Resources
		(.Label                         "这个是对话框标题")
		(.Layout
			(Grid (Rows 1) (Cols 1 0 0 0 0)
				Layout1	
			)
		)
	)
)

(Layout Layout1
	(Components
		(PushButton                     msg_btn)
	)
	(Resources
		(.Resizeable           True)
		(msg_btn.Label          "这是slide1的测试按钮")
		(.Layout
			(Grid (Rows 1) (Cols  1)
				msg_btn
			)
		)
    )
)
