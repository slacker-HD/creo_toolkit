(Layout mainpage
    (Components
        (PushButton                     ok_btn)
        (PushButton                     cancel_btn)
		(Separator                      Separator1)
        (Separator                      Separator2)
		(SubLayout                      Layout1)
    )
	
	(Resources
		(ok_btn.Bitmap              "cl_ok")
        (ok_btn.HelpText            "Complete and exit") 
        (ok_btn.ButtonStyle         3)
        (ok_btn.AttachRight         True)
        (cancel_btn.Bitmap          "cl_cancel")
        (cancel_btn.HelpText        "Cancel and exit")
        (cancel_btn.ButtonStyle     3)
        (cancel_btn.AttachRight     True)
		(Separator1.Orientation         True)
        (Separator1.AttachLeft          False)
        (Separator1.TopOffset           0)
        (Separator1.BottomOffset        0)
        (Separator1.LeftOffset          0)
        (Separator1.RightOffset         0)
        (Separator2.Orientation         True)
        (Separator2.AttachLeft          False)
        (Separator2.TopOffset           0)
        (Separator2.BottomOffset        0)
        (Separator2.LeftOffset          0)
        (Separator2.RightOffset         0)
		
		
		(.Label                         "这个是对话框标题")
		(.Layout
			(Grid (Rows 1) (Cols 1 0 0 0 0)
				Layout1	
				Separator1
				ok_btn
				Separator2
				cancel_btn
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
		(msg_btn.Label          "这是mainpage的测试按钮")
		(.Layout
			(Grid (Rows 1) (Cols  1)
				msg_btn
			)
		)
    )
)
