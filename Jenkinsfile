def getBuildUser(){
    return wrap([$class: 'BuildUser']) { return env.BUILD_USER }
}
def badge(BUILD_USER)
{
    addShortText(text: "${BUILD_NUMBER}", background: 'cyan', border: 1);
    addShortText(text: "${BUILD_USER}", background: 'yellow', border: 1) ;
    addShortText(text: "${GIT_BRANCH}", background: 'pink', border: 1);
    addBadge(icon: "text.gif", text: "Changes", link: "${BUILD_URL}changes");
}
pipeline {
    agent { label 'PHY_BUILD_SERVER' }
    environment{
       //Environment variables to Email notification & Badges
        mail_to="dg.buildnotify.openbmc@axiado.com"
        cc_to="DG.DevOps.All@axiado.com"
        BUILD_USER= getBuildUser()
        build_url = "${env.BUILD_URL}"
        WORKSPACE = "${WORKSPACE}/${BUILD_NUMBER}"
        OPENBMC_DIR = "${WORKSPACE}/openbmc"
    }

    options {
         checkoutToSubdirectory("${BUILD_NUMBER}/openbmc")
    }

    stages {
        stage('Pre-build notification') {
            steps{
				mail to: "${mail_to}",
				cc: "${cc_to}",
				subject: " [CICD] (Build Status -INITIATED) - ${env.JOB_NAME} has been initiated",
				body: """Hi Team, \n\nJob has been started for building the project. \n\nBuild Triggered By:${BUILD_USER} \nProject Name:${env.JOB_NAME} \nBuild URL: ${env.BUILD_URL} \nBuild Number:${env.BUILD_NUMBER} \nBranch Name: ${env.GIT_BRANCH} \nCommit Changes:${RUN_CHANGES_DISPLAY_URL}  \n\nThanks, \nCICD Team"""
			}
		}
        stage('Build') {
            steps {
                script{
                    sh "cd ${OPENBMC_DIR}; ci/scripts/build-bmc.sh"
                }
            }
        }
    }
    post{
       always{
			script{	badge("${BUILD_USER}") }
		}
		success{
	    	mail to: "${mail_to}",
			cc: "${cc_to}",
			subject: "[CICD] (Build Status - SUCCESS ) - ${env.JOB_NAME} has been completed",
			body: """Hi Team, \n\nThis is to notify that the job triggered has been completed with a success status. \n\nBuild Triggered By:${BUILD_USER} \nProject Name:${env.JOB_NAME} \nBuild URL: ${env.BUILD_URL} \nBuild Number:${env.BUILD_NUMBER} \nBranch Name: ${env.GIT_BRANCH} \nDownload log:${RUN_ARTIFACTS_DISPLAY_URL} \n\nThanks, \nCICD Team"""
		}
		failure{
			mail to: "${mail_to}",
			cc: "${cc_to}",
			subject: "[CICD] (Build Status - FAILURE ) - ${env.JOB_NAME} has been completed",
			body: """Hi Team, \n\nThis is to notify that the job triggered has been completed with failure status,Please check the log for further details.  \n\nBuild Triggered By:${BUILD_USER} \nProject Name:${env.JOB_NAME} \nBuild URL: ${env.BUILD_URL} \nBuild Number:${env.BUILD_NUMBER} \nBranch Name: ${env.GIT_BRANCH} \nDownload log:${RUN_ARTIFACTS_DISPLAY_URL} \n\nThanks, \nCICD Team"""
		}
		
	} 
}
