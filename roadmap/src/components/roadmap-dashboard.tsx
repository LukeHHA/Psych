"use client"

import {
  ArrowRightIcon,
  CheckCircle2Icon,
  ClipboardCheckIcon,
  CopyIcon,
  GitBranchIcon,
  Layers3Icon,
  RotateCcwIcon,
  RouteIcon,
  TargetIcon,
  TerminalSquareIcon,
} from "lucide-react"
import { useMemo, useState, useSyncExternalStore } from "react"

import { ThemeToggle } from "@/components/theme-toggle"
import {
  Accordion,
  AccordionContent,
  AccordionItem,
  AccordionTrigger,
} from "@/components/ui/accordion"
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert"
import { Badge } from "@/components/ui/badge"
import { Button } from "@/components/ui/button"
import {
  Card,
  CardAction,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from "@/components/ui/card"
import { Checkbox } from "@/components/ui/checkbox"
import {
  Progress,
  ProgressLabel,
  ProgressValue,
} from "@/components/ui/progress"
import { Separator } from "@/components/ui/separator"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import {
  afterSlice,
  capabilitySnapshot,
  defaultCompletedTaskIds,
  deferredUntilAfterSlice,
  milestones,
  northStar,
  type Milestone,
  type RoadmapTask,
  type WorkKind,
} from "@/data/roadmap"
import { cn } from "@/lib/utils"

const STORAGE_KEY = "psych-roadmap-progress-v1"
const PROGRESS_EVENT = "psych-roadmap-progress"
const DEFAULT_PROGRESS = JSON.stringify(defaultCompletedTaskIds)

function subscribeToProgress(onStoreChange: () => void) {
  window.addEventListener("storage", onStoreChange)
  window.addEventListener(PROGRESS_EVENT, onStoreChange)

  return () => {
    window.removeEventListener("storage", onStoreChange)
    window.removeEventListener(PROGRESS_EVENT, onStoreChange)
  }
}

function getProgressSnapshot() {
  return window.localStorage.getItem(STORAGE_KEY) ?? DEFAULT_PROGRESS
}

function readCompleted(snapshot: string) {
  try {
    const taskIds = JSON.parse(snapshot) as unknown
    return new Set(Array.isArray(taskIds) ? taskIds.filter((id): id is string => typeof id === "string") : defaultCompletedTaskIds)
  } catch {
    return new Set(defaultCompletedTaskIds)
  }
}

function writeCompleted(completed: Set<string>) {
  window.localStorage.setItem(STORAGE_KEY, JSON.stringify([...completed]))
  window.dispatchEvent(new Event(PROGRESS_EVENT))
}

const kindLabels: Record<WorkKind, string> = {
  feature: "Feature",
  test: "Test",
  repair: "Repair",
  integration: "Integration",
}

function milestoneIsDone(milestone: Milestone, completed: Set<string>) {
  return milestone.tasks.every((task) => completed.has(task.id))
}

function milestoneIsReady(milestone: Milestone, completed: Set<string>) {
  return milestone.dependencies.every((dependency) => {
    const required = milestones.find((candidate) => candidate.id === dependency)
    return required ? milestoneIsDone(required, completed) : true
  })
}

function StatusBadge({ milestone, completed }: { milestone: Milestone; completed: Set<string> }) {
  if (milestoneIsDone(milestone, completed)) {
    return (
      <Badge variant="default">
        <CheckCircle2Icon />
        Complete
      </Badge>
    )
  }

  if (milestoneIsReady(milestone, completed)) {
    return <Badge variant="secondary">Ready</Badge>
  }

  return <Badge variant="outline">Queued</Badge>
}

function TaskRow({
  task,
  checked,
  onToggle,
}: {
  task: RoadmapTask
  checked: boolean
  onToggle: (taskId: string, checked: boolean) => void
}) {
  return (
    <label
      htmlFor={task.id}
      className={cn(
        "flex cursor-pointer items-start gap-3 rounded-lg border p-3 transition-colors hover:bg-muted/60",
        checked && "bg-muted/40",
      )}
    >
      <Checkbox
        id={task.id}
        checked={checked}
        onCheckedChange={(value) => onToggle(task.id, value === true)}
        aria-label={`Mark ${task.title} ${checked ? "incomplete" : "complete"}`}
      />
      <span className="flex min-w-0 flex-1 flex-col gap-1.5">
        <span className="flex flex-wrap items-center gap-2">
          <span className={cn("font-medium", checked && "text-muted-foreground line-through")}>
            {task.title}
          </span>
          <Badge variant="outline">{kindLabels[task.kind]}</Badge>
        </span>
        <span className="text-sm leading-relaxed text-muted-foreground">{task.detail}</span>
        <code className="break-words text-xs text-muted-foreground">{task.files.join(" · ")}</code>
      </span>
    </label>
  )
}

function MilestoneCard({
  milestone,
  completed,
  onToggle,
}: {
  milestone: Milestone
  completed: Set<string>
  onToggle: (taskId: string, checked: boolean) => void
}) {
  const completedTasks = milestone.tasks.filter((task) => completed.has(task.id)).length

  return (
    <Card id={milestone.id}>
      <CardHeader>
        <div className="flex flex-wrap items-center gap-2">
          <Badge>{milestone.id.toUpperCase()}</Badge>
          <Badge variant="outline">{milestone.size}</Badge>
          <StatusBadge milestone={milestone} completed={completed} />
        </div>
        <CardTitle className="mt-2 text-xl">{milestone.title}</CardTitle>
        <CardDescription>{milestone.subtitle}</CardDescription>
        <CardAction>
          <span className="font-mono text-xs text-muted-foreground">
            {completedTasks}/{milestone.tasks.length}
          </span>
        </CardAction>
      </CardHeader>
      <CardContent className="flex flex-col gap-4">
        <p className="max-w-3xl leading-relaxed">{milestone.outcome}</p>
        <Progress value={(completedTasks / milestone.tasks.length) * 100}>
          <ProgressLabel className="sr-only">{milestone.title} progress</ProgressLabel>
          <ProgressValue className="sr-only" />
        </Progress>
        <div className="flex flex-wrap gap-1.5">
          <span className="mr-1 text-xs font-medium text-muted-foreground">Builds on</span>
          {milestone.buildOn.map((capability) => (
            <Badge key={capability} variant="secondary">
              {capability}
            </Badge>
          ))}
        </div>
        <div className="flex flex-col gap-2">
          {milestone.tasks.map((task) => (
            <TaskRow
              key={task.id}
              task={task}
              checked={completed.has(task.id)}
              onToggle={onToggle}
            />
          ))}
        </div>
        <Accordion>
          <AccordionItem value={`${milestone.id}-gate`}>
            <AccordionTrigger>Definition of done</AccordionTrigger>
            <AccordionContent>
              <ul className="flex list-disc flex-col gap-2 pl-5 text-muted-foreground">
                {milestone.acceptance.map((item) => (
                  <li key={item}>{item}</li>
                ))}
              </ul>
            </AccordionContent>
          </AccordionItem>
        </Accordion>
      </CardContent>
      <CardFooter>
        <p className="text-xs text-muted-foreground">
          <strong className="text-foreground">Guardrail:</strong> {milestone.guardrail}
        </p>
      </CardFooter>
    </Card>
  )
}

function CapabilityCard({
  title,
  description,
  items,
  variant,
}: {
  title: string
  description: string
  items: string[]
  variant: "default" | "secondary" | "outline"
}) {
  return (
    <Card size="sm">
      <CardHeader>
        <CardTitle>{title}</CardTitle>
        <CardDescription>{description}</CardDescription>
        <CardAction>
          <Badge variant={variant}>{items.length}</Badge>
        </CardAction>
      </CardHeader>
      <CardContent>
        <ul className="flex list-disc flex-col gap-2 pl-4 text-muted-foreground">
          {items.map((item) => (
            <li key={item}>{item}</li>
          ))}
        </ul>
      </CardContent>
    </Card>
  )
}

function DependencyNode({ milestone, completed }: { milestone: Milestone; completed: Set<string> }) {
  return (
    <Card size="sm">
      <CardHeader>
        <div className="flex items-center gap-2">
          <Badge>{milestone.id.toUpperCase()}</Badge>
          <StatusBadge milestone={milestone} completed={completed} />
        </div>
        <CardTitle>{milestone.title}</CardTitle>
        <CardDescription>{milestone.outcome}</CardDescription>
      </CardHeader>
    </Card>
  )
}

export function RoadmapDashboard() {
  const progressSnapshot = useSyncExternalStore(
    subscribeToProgress,
    getProgressSnapshot,
    () => DEFAULT_PROGRESS,
  )
  const completed = useMemo(() => readCompleted(progressSnapshot), [progressSnapshot])
  const [copied, setCopied] = useState(false)

  const allTasks = useMemo(() => milestones.flatMap((milestone) => milestone.tasks), [])
  const completedCount = allTasks.filter((task) => completed.has(task.id)).length
  const progress = Math.round((completedCount / allTasks.length) * 100)
  const currentMilestone =
    milestones.find(
      (milestone) =>
        !milestoneIsDone(milestone, completed) && milestoneIsReady(milestone, completed),
    ) ?? milestones.find((milestone) => !milestoneIsDone(milestone, completed))
  const nextTask = currentMilestone?.tasks.find((task) => !completed.has(task.id))

  function toggleTask(taskId: string, checked: boolean) {
    const updated = new Set(completed)
    if (checked) updated.add(taskId)
    else updated.delete(taskId)
    writeCompleted(updated)
  }

  async function copySessionBrief() {
    if (!currentMilestone || !nextTask) return

    const brief = [
      `Psych vertical slice — ${currentMilestone.id.toUpperCase()}: ${currentMilestone.title}`,
      `Outcome: ${currentMilestone.outcome}`,
      `Next task: ${nextTask.title}`,
      `Detail: ${nextTask.detail}`,
      `Likely files: ${nextTask.files.join(", ")}`,
      `Guardrail: ${currentMilestone.guardrail}`,
      `Done when: ${currentMilestone.acceptance.join(" | ")}`,
    ].join("\n")

    await navigator.clipboard.writeText(brief)
    setCopied(true)
    window.setTimeout(() => setCopied(false), 1800)
  }

  function resetProgress() {
    if (!window.confirm("Reset roadmap progress to the audited baseline?")) return
    writeCompleted(new Set(defaultCompletedTaskIds))
  }

  return (
    <div className="min-h-screen">
      <header className="sticky top-0 z-20 border-b bg-background/90 backdrop-blur-xl">
        <div className="mx-auto flex max-w-7xl items-center justify-between gap-4 px-4 py-3 sm:px-6 lg:px-8">
          <a href="#top" className="flex items-center gap-2 font-semibold tracking-tight">
            <span className="grid size-7 place-items-center rounded-md bg-primary font-mono text-xs text-primary-foreground">
              Ψ
            </span>
            Psych Roadmap
          </a>
          <div className="flex items-center gap-2">
            <Badge variant="outline">Local progress</Badge>
            <ThemeToggle />
          </div>
        </div>
      </header>

      <main id="top" className="mx-auto flex max-w-7xl flex-col gap-10 px-4 py-10 sm:px-6 lg:px-8 lg:py-14">
        <section className="grid items-end gap-8 lg:grid-cols-[minmax(0,1fr)_22rem]">
          <div className="flex flex-col gap-5">
            <Badge variant="secondary" className="w-fit">Vertical slice 01</Badge>
            <div className="flex flex-col gap-3">
              <p className="font-mono text-xs uppercase tracking-[0.24em] text-muted-foreground">
                Goal → proof → missing capability → next goal
              </p>
              <h1 className="max-w-4xl text-balance text-4xl font-semibold tracking-[-0.04em] sm:text-6xl lg:text-7xl">
                Build the smallest complete loop.
              </h1>
              <p className="max-w-3xl text-pretty text-lg leading-relaxed text-muted-foreground sm:text-xl">
                The engine does not need more disconnected systems. It needs one honest path from a project on disk to a scene that survives a restart.
              </p>
            </div>
          </div>
          <Card>
            <CardHeader>
              <CardTitle>Slice progress</CardTitle>
              <CardDescription>{completedCount} of {allTasks.length} tasks complete</CardDescription>
              <CardAction>
                <span className="font-mono text-2xl font-semibold">{progress}%</span>
              </CardAction>
            </CardHeader>
            <CardContent>
              <Progress value={progress}>
                <ProgressLabel className="sr-only">Overall progress</ProgressLabel>
                <ProgressValue className="sr-only" />
              </Progress>
            </CardContent>
          </Card>
        </section>

        <Alert>
          <TargetIcon />
          <AlertTitle>{northStar.title}</AlertTitle>
          <AlertDescription>{northStar.statement}</AlertDescription>
        </Alert>

        <Tabs defaultValue="roadmap">
          <TabsList variant="line" className="w-full justify-start overflow-x-auto">
            <TabsTrigger value="roadmap">
              <RouteIcon data-icon="inline-start" />
              Roadmap
            </TabsTrigger>
            <TabsTrigger value="dependencies">
              <GitBranchIcon data-icon="inline-start" />
              Dependencies
            </TabsTrigger>
            <TabsTrigger value="after">
              <Layers3Icon data-icon="inline-start" />
              After this slice
            </TabsTrigger>
          </TabsList>

          <TabsContent value="roadmap" className="pt-6">
            <div className="grid items-start gap-6 lg:grid-cols-[19rem_minmax(0,1fr)]">
              <aside className="flex flex-col gap-4 lg:sticky lg:top-20">
                <Card>
                  <CardHeader>
                    <Badge variant="secondary" className="w-fit">Work next</Badge>
                    <CardTitle>{nextTask?.title ?? "Slice complete"}</CardTitle>
                    <CardDescription>
                      {currentMilestone
                        ? `${currentMilestone.id.toUpperCase()} · ${currentMilestone.title}`
                        : "Choose the next vertical slice."}
                    </CardDescription>
                  </CardHeader>
                  <CardContent className="flex flex-col gap-3">
                    <p className="leading-relaxed text-muted-foreground">
                      {nextTask?.detail ?? northStar.statement}
                    </p>
                    {nextTask ? (
                      <code className="break-words text-xs text-muted-foreground">
                        {nextTask.files.join(" · ")}
                      </code>
                    ) : null}
                  </CardContent>
                  <CardFooter className="flex gap-2">
                    <Button onClick={copySessionBrief} disabled={!nextTask}>
                      {copied ? <ClipboardCheckIcon data-icon="inline-start" /> : <CopyIcon data-icon="inline-start" />}
                      {copied ? "Copied" : "Copy brief"}
                    </Button>
                    <Button variant="outline" size="icon" onClick={resetProgress} aria-label="Reset progress">
                      <RotateCcwIcon />
                    </Button>
                  </CardFooter>
                </Card>

                <Card size="sm">
                  <CardHeader>
                    <CardTitle>Proof checklist</CardTitle>
                    <CardDescription>The demo only counts when the full loop works.</CardDescription>
                  </CardHeader>
                  <CardContent>
                    <ul className="flex flex-col gap-3">
                      {northStar.proof.map((item) => (
                        <li key={item} className="flex items-start gap-2 text-muted-foreground">
                          <CheckCircle2Icon className="mt-0.5 size-4 shrink-0 text-primary" />
                          <span>{item}</span>
                        </li>
                      ))}
                    </ul>
                  </CardContent>
                </Card>
              </aside>

              <div className="flex min-w-0 flex-col gap-5">
                {milestones.map((milestone) => (
                  <MilestoneCard
                    key={milestone.id}
                    milestone={milestone}
                    completed={completed}
                    onToggle={toggleTask}
                  />
                ))}
              </div>
            </div>

            <section className="mt-12 flex flex-col gap-5">
              <div className="flex flex-col gap-1">
                <p className="font-mono text-xs uppercase tracking-[0.2em] text-muted-foreground">Codebase audit</p>
                <h2 className="text-2xl font-semibold tracking-tight">What exists versus what connects</h2>
              </div>
              <div className="grid gap-4 md:grid-cols-3">
                <CapabilityCard
                  title="Working"
                  description="Keep and build on these paths."
                  items={capabilitySnapshot.working}
                  variant="default"
                />
                <CapabilityCard
                  title="Disconnected"
                  description="Present in code, absent from the loop."
                  items={capabilitySnapshot.disconnected}
                  variant="secondary"
                />
                <CapabilityCard
                  title="Missing"
                  description="Create only what the slice demands."
                  items={capabilitySnapshot.missing}
                  variant="outline"
                />
              </div>
            </section>
          </TabsContent>

          <TabsContent value="dependencies" className="pt-6">
            <div className="grid items-start gap-6 lg:grid-cols-[minmax(0,1fr)_20rem]">
              <Card>
                <CardHeader>
                  <CardTitle className="flex items-center gap-2 text-xl">
                    <GitBranchIcon className="size-5" />
                    Critical path
                  </CardTitle>
                  <CardDescription>
                    Camera and scene work split only after the rendered cube exists, then reunite at restart restoration.
                  </CardDescription>
                </CardHeader>
                <CardContent className="mx-auto flex w-full max-w-3xl flex-col">
                  <DependencyNode milestone={milestones[0]} completed={completed} />
                  <div className="dependency-line" />
                  <DependencyNode milestone={milestones[1]} completed={completed} />
                  <div className="dependency-line" />
                  <DependencyNode milestone={milestones[2]} completed={completed} />
                  <div className="dependency-line" />
                  <div className="dependency-branch">
                    <DependencyNode milestone={milestones[3]} completed={completed} />
                    <DependencyNode milestone={milestones[4]} completed={completed} />
                  </div>
                  <div className="dependency-line" />
                  <DependencyNode milestone={milestones[5]} completed={completed} />
                </CardContent>
              </Card>

              <div className="flex flex-col gap-4">
                <Alert>
                  <TerminalSquareIcon />
                  <AlertTitle>Why this order?</AlertTitle>
                  <AlertDescription>
                    Every milestone ends in observable behavior. If the proof fails, the fault is contained to the latest link.
                  </AlertDescription>
                </Alert>
                <Card size="sm">
                  <CardHeader>
                    <CardTitle>Dependency rule</CardTitle>
                    <CardDescription>Do not start a queued goal because its code looks more interesting.</CardDescription>
                  </CardHeader>
                  <CardContent>
                    <p className="leading-relaxed text-muted-foreground">
                      Finish the acceptance gate, update the checklist, then take the first ready task. That is the entire planning system.
                    </p>
                  </CardContent>
                </Card>
              </div>
            </div>
          </TabsContent>

          <TabsContent value="after" className="pt-6">
            <div className="flex flex-col gap-8">
              <section className="flex flex-col gap-4">
                <div className="flex flex-col gap-1">
                  <p className="font-mono text-xs uppercase tracking-[0.2em] text-muted-foreground">Repeat the loop</p>
                  <h2 className="text-2xl font-semibold tracking-tight">The next four end goals</h2>
                  <p className="max-w-3xl text-muted-foreground">
                    Each goal begins with a user-visible proof, then earns the missing engine capabilities needed to deliver it.
                  </p>
                </div>
                <div className="grid gap-4 md:grid-cols-2">
                  {afterSlice.map((goal) => (
                    <Card key={goal.id}>
                      <CardHeader>
                        <div className="flex items-center gap-2">
                          <Badge>{goal.id.toUpperCase()}</Badge>
                          <Badge variant="outline">After {goal.dependsOn}</Badge>
                        </div>
                        <CardTitle className="mt-2">{goal.title}</CardTitle>
                        <CardDescription>{goal.proof}</CardDescription>
                      </CardHeader>
                      <CardContent>
                        <div className="flex flex-wrap gap-2">
                          {goal.capabilities.map((capability) => (
                            <Badge key={capability} variant="secondary">{capability}</Badge>
                          ))}
                        </div>
                      </CardContent>
                    </Card>
                  ))}
                </div>
              </section>

              <Separator />

              <section className="grid gap-4 lg:grid-cols-[18rem_minmax(0,1fr)]">
                <div>
                  <h2 className="text-xl font-semibold">Explicitly deferred</h2>
                  <p className="mt-2 text-muted-foreground">Good ideas that do not help close slice 01.</p>
                </div>
                <div className="flex flex-wrap content-start gap-2">
                  {deferredUntilAfterSlice.map((item) => (
                    <Badge key={item} variant="outline">{item}</Badge>
                  ))}
                </div>
              </section>
            </div>
          </TabsContent>
        </Tabs>

        <Separator />

        <footer className="flex flex-col justify-between gap-3 pb-6 text-sm text-muted-foreground sm:flex-row sm:items-center">
          <p>Audited against the current Psych engine and editor codebase.</p>
          <a href="#top" className="inline-flex items-center gap-1 font-medium text-foreground hover:underline">
            Back to the goal <ArrowRightIcon className="size-4 -rotate-90" />
          </a>
        </footer>
      </main>
    </div>
  )
}
