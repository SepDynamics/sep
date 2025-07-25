# SEP Dynamics Website Deployment Setup

## Quick Setup Guide

The codechecker script is already configured to auto-update your website. You just need to set up GitHub Pages manually due to OAuth permission restrictions.

### Step 1: Create GitHub Workflow (Manual)

Go to your GitHub repo: https://github.com/SepDynamics/sep

1. Click **Actions** tab
2. Click **New workflow** 
3. Click **set up a workflow yourself →**
4. Name it `deploy-website.yml`
5. Paste this content:

```yaml
name: Deploy SEP Dynamics Website

on:
  push:
    branches: [ master ]
    paths:
      - '.codechecker/**'
      - 'index.html'
      - 'style.css'
      - 'main.js'
      - 'concepts.json'
      - 'logo.png'
  workflow_dispatch:

permissions:
  contents: read
  pages: write
  id-token: write

concurrency:
  group: "pages"
  cancel-in-progress: false

jobs:
  deploy:
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Setup Pages
        uses: actions/configure-pages@v5

      - name: Prepare site content
        run: |
          mkdir -p _site
          cp index.html _site/ 2>/dev/null || true
          cp style.css _site/ 2>/dev/null || true
          cp main.js _site/ 2>/dev/null || true
          cp logo.png _site/ 2>/dev/null || true
          cp concepts.json _site/ 2>/dev/null || true
          
          if [ -d ".codechecker" ]; then
            cp -r .codechecker/* _site/
          fi
          
          cat > _site/deploy_info.json << EOF
          {
            "deployed_at": "$(date -u +"%Y-%m-%dT%H:%M:%SZ")",
            "git_sha": "${{ github.sha }}",
            "git_ref": "${{ github.ref }}",
            "repository": "${{ github.repository }}",
            "workflow_run": "${{ github.run_number }}"
          }
          EOF

      - name: Upload artifact
        uses: actions/upload-pages-artifact@v3
        with:
          path: '_site'

      - name: Deploy to GitHub Pages
        id: deployment
        uses: actions/deploy-pages@v4
```

6. Click **Commit changes**

### Step 2: Enable GitHub Pages

1. Go to **Settings** tab in your repo
2. Scroll down to **Pages** section
3. Under **Source**, select **GitHub Actions**
4. Click **Save**

### Step 3: Point Domain to GitHub Pages

In your DNS settings for sepdynamics.co:
- Add CNAME record: `sepdynamics.co` → `sepdynamics.github.io`
- Or A records pointing to GitHub Pages IPs:
  - `185.199.108.153`
  - `185.199.109.153`  
  - `185.199.110.153`
  - `185.199.111.153`

### Step 4: Test the Setup

Run the codechecker script:
```bash
./run_codechecker.sh
```

This will:
- ✅ Run CodeChecker analysis
- ✅ Generate project status dashboard  
- ✅ Create deployment-ready files in `.codechecker/`
- ✅ Auto-commit and push results
- ✅ Trigger GitHub Pages deployment
- ✅ Update sepdynamics.co automatically

## File Structure After Setup

```
.codechecker/
├── index.html          # Main project dashboard
├── status.html         # Live project status  
├── project_status.json # API data for status
├── html/              # CodeChecker analysis results
└── [other assets]     # CSS, JS, images
```

## URLs After Setup

- **Main Site**: https://sepdynamics.co
- **Live Status**: https://sepdynamics.co/status.html  
- **CodeChecker**: https://sepdynamics.co/html/index.html
- **API Data**: https://sepdynamics.co/project_status.json

The site auto-updates every time you run the codechecker script and push to GitHub!
