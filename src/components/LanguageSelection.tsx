import { ClickAwayListener, MenuItem, MenuList, Paper, Stack, Button, Grow, Popper } from "@mui/material";
import { useEffect, useRef, useState } from "react";
import i18n from "../i18n/config";
import { Languages } from "../Types";


export default function LanguageSelection() {
  const [open, setOpen] = useState(false);
  const [currentLanguage, setCurrentLanguage] = useState(Languages.find(e => e.iso === i18n.language)?.name);
  const anchorRef = useRef<HTMLButtonElement>(null);

  const handleToggle = () => {
    setOpen((currentState) => !currentState);
  };

  function languageClickHandler(languageIsoCode: string): void {
    setCurrentLanguage(Languages.find(e => e.iso === languageIsoCode)?.name);
    i18n.changeLanguage(languageIsoCode);
    localStorage.setItem('language', languageIsoCode);
    setOpen(false);
  }

  const handleClose = (event: Event | React.SyntheticEvent) => {
    if (
      anchorRef.current &&
      anchorRef.current.contains(event.target as HTMLElement)
    ) {
      return;
    }

    setOpen(false);
  };

  function handleListKeyDown(event: React.KeyboardEvent) {
    if (event.key === 'Tab') {
      event.preventDefault();
      setOpen(false);
    } else if (event.key === 'Escape') {
      setOpen(false);
    }
  }

  // return focus to the button when we transitioned from !open -> open
  const prevOpen = useRef(open);
  useEffect(() => {
    if (prevOpen.current === true && open === false) {
      anchorRef.current!.focus();
    }

    prevOpen.current = open;
  }, [open]);

  return (
    <Stack direction="row" spacing={2} id='language-menu'>
      <div>
        <Button
          ref={anchorRef}
          id="composition-button"
          aria-controls={open ? 'composition-menu' : undefined}
          aria-expanded={open ? 'true' : undefined}
          aria-haspopup="true"
          onClick={handleToggle}
          style={{color: 'white', marginLeft: '10px', border: '1px solid white'}}
        >
          {currentLanguage}
        </Button>
        <Popper
          open={open}
          anchorEl={anchorRef.current}
          role={undefined}
          placement="bottom-start"
          transition
          disablePortal
        >
          {({ TransitionProps, placement }) => (
            <Grow
              {...TransitionProps}
              style={{
                transformOrigin:
                  placement === 'bottom-start' ? 'left top' : 'left bottom',
              }}
            >
              <Paper>
                <ClickAwayListener onClickAway={handleClose}>
                  <MenuList
                    autoFocusItem={open}
                    id="composition-menu"
                    aria-labelledby="composition-button"
                    onKeyDown={handleListKeyDown}
                  >
                    {
                      Languages.map(language =>
                        <MenuItem onClick={() => languageClickHandler(language.iso)}>{language.name}</MenuItem>
                      )
                    }
                  </MenuList>
                </ClickAwayListener>
              </Paper>
            </Grow>
          )}
        </Popper>
      </div>
    </Stack>
  );
}