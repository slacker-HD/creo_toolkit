
(Layout NavigatorpaneTest
    (Components
        (Label                          Question)
        (PushButton                     OK)
        (PushButton                     Cancel)
    )

    (Resources
        (Question.Label                 "Dummy Label")
        (OK.Label                       "OK")
        (Cancel.Label                   "Cancel")
        (.Layout
            (Grid (Rows 1 1) (Cols 1 1 1)
                (Pos 1 2)
                Question
                (Pos 2 1)
                OK
                (Pos 2 3)
                Cancel
            )
        )
    )
)
