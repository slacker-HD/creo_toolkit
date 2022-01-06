(Dialog ChangeWorkDirectory
    (Components
        (List WorkDirList)
        (Separator CommitSeparator)
        (PushButton CommitChangeDir)
        (PushButton CommitCancelChangeDir)
    )
    (Resources
        (WorkDirList.TopOffset 5)
        (WorkDirList.BottomOffset 5)
        (WorkDirList.LeftOffset 5)
        (WorkDirList.RightOffset 5)
        (WorkDirList.AttachLeft True)

        (CommitChangeDir.Label "设定为工作目录")
        (CommitChangeDir.TopOffset 5)
        (CommitChangeDir.BottomOffset 5)
        (CommitChangeDir.LeftOffset 5)
        (CommitChangeDir.RightOffset 5)
        (CommitChangeDir.AttachLeft True)

        (CommitCancelChangeDir.Label "取消")
        (CommitCancelChangeDir.TopOffset 5)
        (CommitCancelChangeDir.BottomOffset 5)
        (CommitCancelChangeDir.LeftOffset 5)
        (CommitCancelChangeDir.RightOffset 5)
        (CommitCancelChangeDir.AttachRight True)
        (.TopOffset 5)
        (.BottomOffset 5)
        (.LeftOffset 5)
        (.RightOffset 5)
        (.DefaultButton "CommitChangeDir")
        (.Label "选择工作目录")
        (.Resizeable False)
        (.Layout
            (Grid
                (Rows 1 0 0)
                (Cols 1)
                WorkDirList CommitSeparator
                (Grid
                    (Rows 1)
                    (Cols 1 1)
                    (Pos 1 1)
                    CommitChangeDir
                    (Pos 1 2)
                    CommitCancelChangeDir
                )
            )
        )
    )
)
