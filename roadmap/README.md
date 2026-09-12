# Psych Roadmap

A small local Next.js app that turns the current Psych codebase audit into a goal-oriented development loop.

The first vertical slice is intentionally concrete:

> Load a project, open a scene, render a textured 3D object, move a camera, close the editor, and reopen into the same scene.

## Run it

```bash
cd roadmap
npm install
npm run dev
```

Open [http://localhost:3001](http://localhost:3001).

## How to use it

- Start with the **Work next** card rather than choosing a system in isolation.
- Copy the session brief into a development note or Codex task when starting work.
- Check off tasks as their milestone acceptance gates pass.
- Use **Dependencies** to see what can run in parallel and what remains blocked.
- Use **After this slice** to keep attractive but premature work out of the current goal.

Task progress is stored only in the browser under `psych-roadmap-progress-v1`. There is no account, database, analytics, or deployment requirement. The theme defaults to the operating-system setting and can be changed to light, dark, or system from the header.

## Edit the roadmap

The audited goals and task data live in [`src/data/roadmap.ts`](src/data/roadmap.ts). The interactive UI lives in [`src/components/roadmap-dashboard.tsx`](src/components/roadmap-dashboard.tsx).

## Verify it

```bash
npm run lint
npm run build
```
