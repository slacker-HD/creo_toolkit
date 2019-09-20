(Dialog originview
    (Components
        (SubLayout Content)
        (Separator CommitSeparator)
        (PushButton Exit)
        (PushButton Set)
        (Label LabelCalendar)
        (Label LabelString)
        (InputPanel InputCalendar)
        (InputPanel InputString)
    )
    (Resources
        (Exit.Label "退出")
        (Exit.TopOffset 5)
        (Exit.BottomOffset 5)
        (Exit.LeftOffset 5)
        (Exit.RightOffset 5)
        (Exit.AttachRight True)
        (Set.Label "设置MFC控件值")
        (Set.TopOffset 5)
        (Set.BottomOffset 5)
        (Set.LeftOffset 5)
        (Set.RightOffset 5)
        (Set.AttachLeft True)
        (LabelCalendar.Label "选择的日期：")
        (LabelCalendar.AttachLeft True)
        (LabelCalendar.TopOffset 5)
        (LabelCalendar.BottomOffset 5)
        (LabelCalendar.LeftOffset 5)
        (LabelCalendar.RightOffset 5)
        (LabelString.Label "输入文字")
        (LabelString.AttachLeft True)
        (LabelString.TopOffset 5)
        (LabelString.BottomOffset 5)
        (LabelString.LeftOffset 5)
        (LabelString.RightOffset 5)


        (.TopOffset 5)
        (.BottomOffset 5)
        (.LeftOffset 5)
        (.RightOffset 5)
        (.DefaultButton "Open")
        (.Label "this is a test dialog")
        (.Resizeable False)
        (.Layout
            (Grid
                (Rows 1 0 0 0)
                (Cols 1)
                Content CommitSeparator
                (Grid
                    (Rows 0 0)
                    (Cols 1 1)
                    LabelCalendar InputCalendar LabelString InputString
                )
                (Grid
                    (Rows 1)
                    (Cols 1 1)
                    (Pos 1 1)
                    Set
                    (Pos 1 2)
                    Exit
                )
            )
            
        )
    )
)

(Layout Content
    (Components
 	    (DrawingArea	 Preview)	 
    )
    (Resources
         (Preview.DrawingHeight                  400)
         (Preview.DrawingWidth                  400)
         (Preview.TopOffset				 10)
         (Preview.LeftOffset				 4)
         (Preview.RightOffset				 2)
        (.Layout
            (Grid
                (Rows 0)
                (Cols 1)
                Preview
            )
        )
    )
)
