"use client"

import { CheckIcon, LaptopIcon, MoonIcon, SunIcon } from "lucide-react"
import { useTheme } from "next-themes"
import { useSyncExternalStore } from "react"

import { Button } from "@/components/ui/button"
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuLabel,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu"

const emptySubscribe = () => () => undefined

export function ThemeToggle() {
  const mounted = useSyncExternalStore(emptySubscribe, () => true, () => false)
  const { resolvedTheme, setTheme, theme } = useTheme()
  const ThemeIcon = mounted && resolvedTheme === "dark" ? MoonIcon : SunIcon

  return (
    <DropdownMenu>
      <DropdownMenuTrigger
        render={
          <Button
            variant="outline"
            size="icon"
            aria-label="Choose color theme"
          />
        }
      >
        <ThemeIcon />
      </DropdownMenuTrigger>
      <DropdownMenuContent align="end" className="w-40">
        <DropdownMenuGroup>
          <DropdownMenuLabel>Appearance</DropdownMenuLabel>
          <DropdownMenuItem onClick={() => setTheme("light")}>
            <SunIcon />
            Light
            {mounted && theme === "light" ? <CheckIcon className="ml-auto" /> : null}
          </DropdownMenuItem>
          <DropdownMenuItem onClick={() => setTheme("dark")}>
            <MoonIcon />
            Dark
            {mounted && theme === "dark" ? <CheckIcon className="ml-auto" /> : null}
          </DropdownMenuItem>
          <DropdownMenuItem onClick={() => setTheme("system")}>
            <LaptopIcon />
            System
            {mounted && theme === "system" ? <CheckIcon className="ml-auto" /> : null}
          </DropdownMenuItem>
        </DropdownMenuGroup>
      </DropdownMenuContent>
    </DropdownMenu>
  )
}
